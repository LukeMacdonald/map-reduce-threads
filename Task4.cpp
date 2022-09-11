#include "Task4.h"
// Global variables that all threads can access
const int GRACEFUL_SECONDS = 10;
bool GRACEFUL_EXIT = false;
bool THREAD_SIGNAL = false;
pthread_mutex_t mutex;
pthread_cond_t cond;
std::vector<std::string> Global;
std::queue<std::string> stringCache[THREAD_NUM];
std::vector<std::string> sortingCache; 
// Object that handles all outputs (error and stdout)
OutputHandler output_hander;

int main(int argc, char * argv[]) {
    // starts recording execution time for entire program
    clock_t start,end;
    start = clock();
    output_hander.open_files("Task4Files"); 
    signal(SIGALRM, graceful_exit_handler); 
    alarm(GRACEFUL_SECONDS);
    
    //Ensure args present
    if(argv[1] == nullptr || argv[2] == nullptr) {
        output_hander.print_error("Invalid usage: './Task4 INPUTFILE.txt OUTPUTFILE.txt'"); 
        return EXIT_FAILURE; 
    }

     // First command line argument is the name of dirty file
    std::string input_file = argv[1];

    // Second command line argument is the name of the clean file
    std::string output_file= argv[2];

    for(int i = 3;i < 16; i++){
        // Creates Fifo file
        if(mkfifo(("Task4Files/Fifo/FIFO" + std::to_string(i)).c_str(), 0777) == -1){
            if(errno != EEXIST){
                output_hander.print_error("Could Not Create Fifo File\n");
            }
        }
    }
    output_hander.print_log("# Using input file: " + input_file);

    // Function to filter to dirty file into the clean file.
    std::string filter = "Task4Files/filtered/filter.txt";
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

    // Create Map Thread
    if(pthread_create(&map, nullptr, &map4, nullptr) != 0){
        output_hander.print_error("Failed to create thread\n");
    }
    
    // Create Reduce Thread
    if(pthread_create(&reduce, nullptr, &reduce4, &output_file) != 0){
        output_hander.print_error("Failed to create thread\n");
    }
   
    // Join Map Thread
    if(pthread_join(map,nullptr)!=0){
        output_hander.print_error("Failed to join map thread thread\n");
    }
    if(pthread_join(reduce, nullptr)!=0){
        output_hander.print_error("Failed to join reduce thread thread\n");
    }

    //Destory the reduce signalling mutex and cond
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    //Task3 Complete
    output_hander.print_log("# Task 4 Finish, Outputs in 'Task4Files/output/' directory"); 
    std::cout << "Task 4 Finish Completed!\nFind ouput file in 'Task4Files/output' directory as '" 
    << output_file <<  "'\n";
    // stops and recoords execution time for entire program
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_hander.print_exec_time("Total",time_taken);
    output_hander.close_files();
    return EXIT_SUCCESS;
}
void *map4(void *args)
{
    // starts recording execution time for map4 thread
    clock_t start,end;
    start = clock();
    std::vector<std::vector<int> > index_array(THREAD_NUM, std::vector<int>());
    MapParams params[THREAD_NUM];
    pthread_t threads[THREAD_NUM];
    std::vector<int> index_sizes;

    //Step 1
    output_hander.print_log("Created index'd array");
    for (int i = 0; i < Global.size();i++){
        int length = Global[i].length();
        index_array.at(length - ARRAY_OFFSET).push_back(i);
    }

    // CREATE THREADS
    for(int i = 0; i < THREAD_NUM;i++){
        int size =index_array[i].size();
        index_sizes.push_back(size);
    }
    
    sort(index_sizes.begin(),index_sizes.end(), sort_indexes);
    std::vector<int> sorted_indexes[THREAD_NUM];

    for(int i = 0; i < THREAD_NUM;i++){
        for(int j = 0; j < THREAD_NUM;j++){
            if((int)index_array[j].size() == index_sizes[i]){
                sorted_indexes[i] = index_array[j];
            }
        }
    }
    
    pthread_attr_t tattr[THREAD_NUM];
    sched_param param;
    int max = sched_get_priority_max(SCHED_RR);
    int min = sched_get_priority_min(SCHED_RR);
    int step = (max - min)/THREAD_NUM;
    int priority = max;

    for(int i = 0; i < THREAD_NUM;i++){
        pthread_attr_init (&tattr[i]);
        pthread_attr_setschedpolicy(&tattr[i], SCHED_RR);
        param.sched_priority =  priority;
        pthread_attr_setschedparam (&tattr[i], &param);
        pthread_attr_setinheritsched (&tattr[i], PTHREAD_INHERIT_SCHED);
        priority -= step;
    }

    //Step 3
    output_hander.print_log("Map Creating " + std::to_string(THREAD_NUM) + " threads");
    for (int i = 0; i < THREAD_NUM; i++)
    {
        params[i].word_length = Global[sorted_indexes[i][0]].size() - 3; 
        params[i].indexes = sorted_indexes[i];
        //params[i].indexes = index_array.at(i);

        int thread_return = pthread_create(&threads[i], &tattr[i], pthread_write, &params[i]);
        if (thread_return) { return NULL; }
    }

    //Step 4
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(threads[i], NULL);
    }
    output_hander.print_log("Closed mapping FIFO and threads");

    //Step 5
    output_hander.print_log("Mapping complete, reduce signaled");
    pthread_mutex_lock(&mutex);
    THREAD_SIGNAL = true;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
    // stops and records execution time for map4 thread
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_hander.print_exec_time("Map4",time_taken);

    //Step 6
    output_hander.print_log("## Mapping complete, FIFO's created in 'FIFOFiles' directory");
    return NULL;
}
void* reduce4(void* args) {
    clock_t start,end;
    start = clock();
    FIFOThreadParams params[THREAD_NUM];
    pthread_t fifoThread[THREAD_NUM];
    std::string file_name = "Task4Files/output/" + *(std::string*)args;
   
    output_hander.print_log("Reduce Creating " + std::to_string(THREAD_NUM) + " threads");
    for (int i = 0; i < THREAD_NUM; i++)
    {
        params[i].index = i;
        params[i].stored_strings = &stringCache[i]; 

        int thread_return = pthread_create(&fifoThread[i], NULL, pthread_read, &params[i]);
        if (thread_return) { return NULL; }
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(fifoThread[i], NULL);
    }
    output_hander.print_log("Reduce threads finished"); 

    output_hander.print_log("Reduce waiting for mapping signal"); 
    pthread_mutex_lock(&mutex);
    while (!THREAD_SIGNAL) {
        pthread_cond_wait(&cond, &mutex);
    }
    output_hander.print_log("Mapping signal recieved reduce working..."); 

    output_hander.print_log("Reduce starting merge sort...");
    for (int i = 0; i < THREAD_NUM && !GRACEFUL_EXIT; i++)  {
        sortingCache.push_back(stringCache[i].front()); 
        stringCache[i].pop();
    }
     std::ofstream outfile(file_name);


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
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_hander.print_exec_time("Reduce4",time_taken);
    output_hander.print_log("## Reducing complete, single reduced filter files in 'Output/Task4' directory as '" + file_name + "'");
    return NULL; 
}
void *pthread_write(void *args) {
    clock_t start,end;
    start = clock();
    struct MapParams *mapData = (struct MapParams *)args;
    std::string word_size = std::to_string(mapData->word_length + ARRAY_OFFSET);
    output_hander.print_log("Map Thread" + word_size + " Has Started Execution!");
    std::string fifo_name = "Task4Files/Fifo/FIFO" + word_size;
    
    sort(mapData->indexes.begin(), mapData->indexes.end(), sort_global); 

    // Create/Open FIFO
    output_hander.print_log("Opening Fifo File " + fifo_name + " For Write!");
    int fd = open(fifo_name.c_str(), O_WRONLY);
    if(fd  == -1) { 
        output_hander.print_error("Fifo File " + fifo_name + " Has Failed to Open!");
        return NULL; 
    }

    // Write each of the index's for that length to the pipe
    output_hander.print_log("Writing Strings out to " + fifo_name);
    for(int i = 0; i < (int)mapData->indexes.size() && !GRACEFUL_EXIT; i++) {
        if (write(fd, &mapData->indexes.at(i), sizeof(int)) == -1) {
            output_hander.print_error("Error writing to FIFO File");
            return NULL;
        };
    };
    output_hander.print_log("Writing Strings out to " + fifo_name + " Has Completed!");
    // Close the handle, return the thread
    close(fd); 
    output_hander.print_log("Closing Fifo File " + fifo_name + " For Write!");
    output_hander.print_log("Map Thread" + word_size + " Has Finished Execution!"); 
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_hander.print_exec_time("Map Thread"+std::to_string(mapData->word_length + ARRAY_OFFSET),time_taken);
    return NULL;
}
void *pthread_read(void *args)  {
    
    clock_t start,end;
    start = clock();
    
    struct FIFOThreadParams *mapData = (struct FIFOThreadParams *)args;
    std::string word_size = std::to_string(mapData->index + ARRAY_OFFSET);
    output_hander.print_log("Reduce Thread" + word_size + " Has Started Execution!");
    
    // Create/Open FIFO
    std::string fifo_name = "Task4Files/Fifo/FIFO" + word_size;
    output_hander.print_log("Opening Fifo File " + fifo_name + " For Read!");
    int fd = open(fifo_name.c_str(), O_RDONLY);
    if(fd  == -1) { 
        output_hander.print_error("Fifo File " + fifo_name + " Has Failed to Open!");
        return NULL; }

    int index; 
    // Read each of the index's for that length to the pipe and push them to reduce stringCache memory
    output_hander.print_log("Reading Strings out to " + fifo_name);
    while(read(fd , &index, sizeof(int)) > 0 && !GRACEFUL_EXIT) {
        mapData->stored_strings->push(Global.at(index));
    }
    output_hander.print_log("Reading Strings out to " + fifo_name + " Has Completed!");

    //Close the handle, return the thread
    close(fd);
    output_hander.print_log("Closing Fifo File " + fifo_name + " For Read!");
    output_hander.print_log("Reduce Thread" + word_size + " Has Finished Execution!"); 
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_hander.print_exec_time("Reduce Thread"+std::to_string(mapData->index + ARRAY_OFFSET),time_taken);
    return NULL;
}
bool sort_global(int index1, int index2)
{
   return Global[index1].substr(2,-1) < Global[index2].substr(2,-1);
}
void graceful_exit_handler(int seconds) {
    output_hander.print_error("Program running to long, " + std::to_string(GRACEFUL_SECONDS) + "s set as the limit, exiting");
    GRACEFUL_EXIT = true; 
}