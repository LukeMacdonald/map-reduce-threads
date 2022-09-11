#include "Task3.h"
#include <chrono>

const int GRACEFUL_SECONDS = 10;
bool GRACEFUL_EXIT = false;
bool THREAD_SIGNAL = false;

pthread_mutex_t mutex;
pthread_cond_t cond;
std::vector<std::string> Global;
std::queue<std::string> stringCache[THREAD_NUM];
std::vector<std::string> sortingCache; 
OutputHandler r;

int main(int argc, char * argv[]) { 
    clock_t start,end;
    start = clock();
    
    r.open_files("Task3Files");
    signal(SIGALRM, graceful_exit_handler); 
    alarm(GRACEFUL_SECONDS);
    std::ofstream error_file("Task3/Files/output/error.txt");
    std::ofstream log_file("Task3/Files/output/log.txt");

    //Ensure args present
    if(argv[1] == nullptr || argv[2] == nullptr) {
        r.print_error("Invalid usage: './Task3 INPUTFILE.txt OUTPUTFILE.txt'"); 
        return EXIT_FAILURE; 
    }

     // First command line argument is the name of dirty file
    std::string input_file = argv[1];

    // Second command line argument is the name of the clean file
    std::string output_file= argv[2];

    for(int i = 3;i < 16; i++){
        // Creates Fifo file
        if(mkfifo(("Task3Files/Fifo/FIFO" + std::to_string(i)).c_str(), 0777) == -1){
            if(errno != EEXIST){
                r.print_error("Could Not Create Fifo File\n");
            }
        }
    }

    r.print_log("# Using input file: " + input_file);

    // Function to filter to dirty file into the clean file.
    std::string filter = "Task3Files/filtered/filter.txt";
    task1Filter(input_file,filter);

    //Read the filtered file into global array memory
    std::ifstream input; 
    input.open(filter); 
    
    for(std::string line; std::getline(input, line);) {
        Global.push_back(line); 
    }

    //Close the input file stream
    input.close(); 

    //Create/Init thread properties, mutex and cond used for mapping to signal reduce
    pthread_t map; 
    pthread_t reduce; 
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    //Create the mapping thread, return if error
    int thread_map_return = pthread_create(&map, NULL, map3, NULL); 
    if (thread_map_return) { return EXIT_FAILURE; }
    
    //Create the reduce thread, return if error
    int thread_reduce_return = pthread_create(&reduce, NULL, reduce3, &output_file); 
    if (thread_reduce_return) { return EXIT_FAILURE; }

    //Process waits and joins when the threads have returned
    pthread_join(reduce, NULL); 
    pthread_join(map, NULL); 

    //Destory the reduce signalling mutex and cond
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    r.print_exec_time("Total",time_taken);

    //Task3 Complete
    std::cout << "Task 4 Finish Completed!\nOutput File is located in in 'Task4Files/output' directory as '" 
    << output_file <<  "'\n";
    r.close_files();
    return EXIT_SUCCESS;
}
void *map3(void *args)
{
    clock_t start, end;
    start = clock();
    std::vector<std::vector<int>> index_array(13, std::vector<int>());
    MapParams params[THREAD_NUM];
    pthread_t threads[THREAD_NUM];

    //Step 1
    r.print_log("Created index'd array");
    int counter = 0;
    for (std::string string : Global)  {
        int length = string.length();
        index_array.at(length - ARRAY_OFFSET).push_back(counter);
        counter++;
    }

    //Step 2
    for(int i = 0; i < THREAD_NUM; i++) {
        sort(index_array.at(i).begin(), index_array.at(i).end(), sort_global); 
    }

    //Step 3
    r.print_log("Map Creating " + std::to_string(THREAD_NUM) + " threads");
    for (int i = 0; i < THREAD_NUM; i++)
    {
        params[i].word_length = i; 
        params[i].indexes = index_array.at(i);

        int thread_return = pthread_create(&threads[i], NULL, pthread_write, &params[i]);
        if (thread_return) { return NULL; }
    }

    //Step 4
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(threads[i], NULL);
    }
    r.print_log("Closed mapping FIFO and threads");

    //Step 5
    r.print_log("Mapping complete, reduce signaled");
    pthread_mutex_lock(&mutex);
    THREAD_SIGNAL = true;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);

    //Step 6
    r.print_log("## Mapping complete, FIFO's created in 'FIFOFiles' directory");
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    r.print_exec_time("Map3",time_taken);
    return NULL;
}
void* reduce3(void* args) {
    clock_t start, end;
    start = clock();
    FIFOThreadParams params[THREAD_NUM];
    pthread_t fifoThread[THREAD_NUM];
    std::string file_name = "Task3Files/output/" + *(std::string*)args;
   
    // Step 1
    r.print_log("Reduce Creating " + std::to_string(THREAD_NUM) + " threads");
    for (int i = 0; i < THREAD_NUM; i++)
    {
        params[i].index = i;
        params[i].stored_strings = &stringCache[i]; 

        int thread_return = pthread_create(&fifoThread[i], NULL, pthread_read, &params[i]);
        if (thread_return) { return NULL; }
    }

    // Step 2
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(fifoThread[i], NULL);
    }
    r.print_log("Reduce threads finished"); 

    // Step 2
    r.print_log("Reduce waiting for mapping signal"); 
    pthread_mutex_lock(&mutex);
    while (!THREAD_SIGNAL) {
        pthread_cond_wait(&cond, &mutex);
    }
    r.print_log("Mapping signal recieved reduce working..."); 

    r.print_log("Reduce starting merge sort...");
    for (int i = 0; i < THREAD_NUM && !GRACEFUL_EXIT; i++)  {

        sortingCache.push_back(stringCache[i].front()); 
        stringCache[i].pop();
    }
     std::ofstream outfile(file_name);
    
    // Step 4,5,6,7,8,9
    while(!sortingCache.empty() && !GRACEFUL_EXIT) {
        sort(sortingCache.begin(), sortingCache.end(), sort_string);

        std::string line = sortingCache.front();
        outfile <<  line << "\n"; 
        pop_front(sortingCache); 
        if(!stringCache[line.length() - ARRAY_OFFSET].empty()){
            sortingCache.push_back(stringCache[line.length() - ARRAY_OFFSET].front());
            stringCache[line.length() - ARRAY_OFFSET].pop();
        }
    }
    //Step 10
    outfile.close();
    r.print_log("## Reducing complete, single reduced filter files in 'Output/Task3' directory as '" + file_name + "'");
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    r.print_exec_time("Reduce3",time_taken);
    return NULL; 
}
void *pthread_write(void *args) {
    clock_t start, end;
    start = clock();
    struct MapParams *mapData = (struct MapParams *)args;
    std::string word_size = std::to_string(mapData->word_length + ARRAY_OFFSET);
    r.print_log("Map Thread" + word_size + " Has Started Execution!");
    std::string fifo_name = "Task3Files/Fifo/FIFO" + word_size;

    // Create/Open FIFO
    r.print_log("Opening Fifo File " + fifo_name + " For Write!");
    int fd = open(fifo_name.c_str(), O_WRONLY);
    if(fd  == -1) { 
        r.print_error("Fifo File " + fifo_name + " Has Failed to Open!");
        return NULL; 
    }

    // Write each of the index's for that length to the pipe
     r.print_log("Writing Strings out to " + fifo_name);
    for(int index = 0; index < (int)mapData->indexes.size() && !GRACEFUL_EXIT; index++) {
        if (write(fd, &mapData->indexes.at(index), sizeof(int)) == -1) {
            r.print_error("Error writing to FIFO File");
            return NULL;
        };
    };
    r.print_log("Writing Strings out to " + fifo_name + " Has Completed!");
    // Close the handle, return the thread
    close(fd); 
    r.print_log("Closing Fifo File " + fifo_name + " For Write!");
    r.print_log("Map Thread" + word_size + " Has Finished Execution!"); 
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    r.print_exec_time("Map Thread"+std::to_string(mapData->word_length + ARRAY_OFFSET),time_taken);
    return NULL;
}
void *pthread_read(void *args)  {
    clock_t start, end;
    start = clock();
    struct FIFOThreadParams *mapData = (struct FIFOThreadParams *)args;
    std::string word_size = std::to_string(mapData->index + ARRAY_OFFSET);
    r.print_log("Reduce Thread" + word_size + " Has Started Execution!");
    std::string fifo_name = "Task3Files/Fifo/FIFO" + word_size;
  
    // Create/Open FIFO
    r.print_log("Opening Fifo File " + fifo_name + " For Read!");
    int fd = open(fifo_name.c_str(), O_RDONLY);
    if(fd  == -1) { 
        r.print_error("Fifo File " + fifo_name + " Has Failed to Open!");
        return NULL; }

    int index; 
    // Read each of the index's for that length to the pipe and push them to reduce stringCache memory
    r.print_log("Reading Strings out to " + fifo_name);
    while(read(fd , &index, sizeof(int)) > 0 && !GRACEFUL_EXIT) {
        mapData->stored_strings->push(Global.at(index));
    }
    r.print_log("Reading Strings out to " + fifo_name + " Has Completed!");

    //Close the handle, return the thread
    close(fd);
    r.print_log("Closing Fifo File " + fifo_name + " For Read!");
    r.print_log("Reduce Thread" + word_size + " Has Finished Execution!"); 
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    r.print_exec_time("Reduce Thread"+std::to_string(mapData->index + ARRAY_OFFSET),time_taken);
    return NULL;
}
bool sort_global(int index1, int index2)
{
   return Global[index1].substr(2,-1) < Global[index2].substr(2,-1);
}
void graceful_exit_handler(int seconds) {
    r.print_error("Program running to long, " + std::to_string(GRACEFUL_SECONDS) + "s set as the limit, exiting");
    GRACEFUL_EXIT = true; 
}