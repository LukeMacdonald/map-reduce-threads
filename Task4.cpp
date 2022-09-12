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
OutputHandler output_handler;

int main(int argc, char * argv[]) {
    // starts recording execution time for entire program
    clock_t start,end;
    start = clock();
    output_handler.open_files("Task4Files"); 
    signal(SIGALRM, graceful_exit_handler); 
    alarm(GRACEFUL_SECONDS);
    
    //Ensure args present
    if(argv[1] == nullptr || argv[2] == nullptr) {
        output_handler.print_error("Invalid usage: './Task4 INPUTFILE.txt OUTPUTFILE.txt'"); 
        return EXIT_FAILURE; 
    }

     // First command line argument is the name of dirty file
    std::string input_file = argv[1];

    // Second command line argument is the name of the clean file
    std::string output_file= argv[2];

    for(int i = 3;i < 16; i++){
        std::string file = "Task4Files/Fifo/FIFO" + std::to_string(i);
        char file_name[file.length() + 1]; 
        strcpy(file_name,file.c_str());
    
        // Creates Fifo file
        if(mkfifo(file_name, 0777) == -1){
            if(errno != EEXIST){
                perror("Could Not Create Fifo File\n");
            }
        }
    }
    output_handler.print_log("# Using input file: " + input_file);

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
        output_handler.print_error("Failed to create thread\n");
    }
    
    // Create Reduce Thread
    if(pthread_create(&reduce, nullptr, &reduce4, &output_file) != 0){
        output_handler.print_error("Failed to create thread\n");
    }
   
    // Join Map Thread
    if(pthread_join(map,nullptr)!=0){
        output_handler.print_error("Failed to join map thread thread\n");
    }
    if(pthread_join(reduce, nullptr)!=0){
        output_handler.print_error("Failed to join reduce thread thread\n");
    }

    //Destory the reduce signalling mutex and cond
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    //Task3 Complete
    output_handler.print_log("# Task 4 Finish, Outputs in 'Task4Files/output/' directory"); 
    std::cout << "Task 4 Finish Completed!\nFind ouput file in 'Task4Files/output' directory as '" 
    << output_file <<  "'\n";
    // stops and recoords execution time for entire program
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Total",time_taken);
    output_handler.close_files();
    return EXIT_SUCCESS;
}
void *map4(void *args)
{
    // Starts the time for measuring execution time 
    // of map4 program
    clock_t start, end;
    start = clock();
    // Declares an array that will contain vectors for each word length
    std::vector<int> index_array[THREAD_NUM];
    // Declares an array containing all the parameters for the different
    // mapping threads
    MapParams params[THREAD_NUM];
    // Declares an array containing all the threads
    pthread_t map_threads[THREAD_NUM];
    // Declares a vector of all index sizes
    std::vector<int> index_sizes;

    // Addsall indexes to the corresponding vectors
    output_handler.print_log("Created all index arrays");
    for (int i = 0; i < (int)Global.size();i++){
        // Gets size of word
        int length = Global[i].length();
        // Subtracts the offset of the array to get the 
        // proper index of that length (subtracts 3)
        index_array[length - ARRAY_OFFSET].push_back(i);
    }
  
    // Add all the index sizes to the corresponding index 
    for(int i = 0; i < THREAD_NUM;i++){
        int size =index_array[i].size();
        index_sizes.push_back(size);
    }
    // Sorts the indexe sizes from largest to smallest
    sort(index_sizes.begin(),index_sizes.end(), sort_indexes);
    // Declares a vector that will contain the new order of 
    // indexes based on the sizes
    std::vector<int> sorted_indexes[THREAD_NUM];
 
    // Initliases the new array of index vectors from largest size to 
    // the smallest size
    for(int i = 0; i < THREAD_NUM;i++){
        for(int j = 0; j < THREAD_NUM;j++){
            // Checks if the size of current vector is the size of the
            // currently accessed index size
            if((int)index_array[j].size() == index_sizes[i]){
                sorted_indexes[i] = index_array[j];
            }
        }
    }
    
    // Declares array of thread attributes for each thread
    pthread_attr_t tattr[THREAD_NUM];
    // Declares varaible for scheduling parameter of thread
    sched_param param;
    // Gets max priority value for the round robin scheduling algorithm
    int max = sched_get_priority_max(SCHED_RR);
    // Gets min priority value for the round robin scheduling algorithm
    int min = sched_get_priority_min(SCHED_RR);
    // Gets step value that will allow the evenly setting up of priority 
    // of all threads from min to max with min being equal to the vector
    // of most indexes
    int step = (max - min)/THREAD_NUM;
    // sets the first priority to the min
    int priority = min;
    // Creates the 13 different thread attributes
    for(int i = 0; i < THREAD_NUM;i++){
        pthread_attr_init (&tattr[i]);
        // Sets scheduling policy as round robin
        pthread_attr_setschedpolicy(&tattr[i], SCHED_RR);
        // assigns the priority value of that attribute to schedling parameter
        param.sched_priority =  priority;
        // Sets the scheduling parameter to the thread attribute
        pthread_attr_setschedparam (&tattr[i], &param);
        // Sets the inherit scheduler attribute
        pthread_attr_setinheritsched (&tattr[i], PTHREAD_EXPLICIT_SCHED);
        // Increases the priority based on the previously calcuated step
        priority += step;
    }

    output_handler.print_log("Map Creating " + std::to_string(THREAD_NUM) + " threads");
    for (int i = 0; i < THREAD_NUM; i++)
    {   
        // stores index of store in map thread parameter
        params[i].word_index = Global[sorted_indexes[i][0]].size() - 3; 
        // stores the index array in map thread parameter
        params[i].indexes = sorted_indexes[i];
        // creates the 13 map pthread
        int thread_return = pthread_create(&map_threads[i], &tattr[i], pthread_write, &params[i]);
        if (thread_return) { 
            return NULL; 
        }
    }

     // Waits for thread termination
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(map_threads[i], NULL);
    }
    output_handler.print_log("Closed mapping FIFO and threads");

    // Using mutex and cond to signal reduce thread onces mapping completed
    output_handler.print_log("Mapping complete, reduce signaled");
    pthread_mutex_lock(&mutex);
    // Sets to global bool to true to allow the reduce
    // thread to unlock and continue execution
    THREAD_SIGNAL = true;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);

    // Records the time after at the end of map3 and
    // output the exectution time for map3.
    output_handler.print_log("## Mapping complete, FIFO's created in 'FIFOFiles' directory");
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Map4",time_taken);
    return NULL;
}
void* reduce4(void* args) {
    // Starts the time for measuring execution time 
    // of reduce4 program
    clock_t start, end;
    start = clock();
    // Declares an array containing all the parameters for the different
    // reducing threads
    ReduceParams params[THREAD_NUM];
    // Declares an array containing all the reduce threads
    pthread_t reduce_threads[THREAD_NUM];
    // Declares an array of queues that will be used to store
    // the data read from each fifo file.
    std::queue<std::string> fifo_queue[THREAD_NUM];
    // Declares an array that will store at most 13 words
    // that will be sorted and written to output
    std::vector<std::string> sorting_list; 
    // Gets output filename from parameter arguments
    std::string file_name = "Task3Files/output/" + *(std::string*)args;
   
    // Create all the reduce threads and their parameters
    output_handler.print_log("Reduce Creating " + std::to_string(THREAD_NUM) + " threads");
    for (int i = 0; i < THREAD_NUM; i++)
    {
        // stores index of store in reduce thread parameter
        params[i].index = i;
        // stores the fifo queue in reduce thread parameter
        params[i].stored_strings = &fifo_queue[i]; 
        // creates the 13 reduce thread
        int thread_return = pthread_create(&reduce_threads[i], NULL, pthread_read, &params[i]);
        if (thread_return) { return NULL; }
    }

    // waits for the 13 reduce threads to terminate
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(reduce_threads[i], NULL);
    }
    output_handler.print_log("Reduce threads finished"); 

    // Reduce3 gets stuck in loop until the mapping is done and signals
    // for reduce3 to continue with execution
    output_handler.print_log("Reduce waiting for mapping signal"); 
    pthread_mutex_lock(&mutex);
    while (!THREAD_SIGNAL) {
        pthread_cond_wait(&cond, &mutex);
    }
    output_handler.print_log("Mapping signal recieved reduce working..."); 
    // After receiving the signal from map4 the reduce4 gets the first item from each
    // file
    output_handler.print_log("Reduce starting merge sort...");
    for (int i = 0; i < THREAD_NUM && !GRACEFUL_EXIT; i++)  {
        // Adds the first element of the fifo queue to the array 
        // containing the 13 words from files
        sorting_list.push_back(fifo_queue[i].front()); 
        // Removes the first element from its queue
        fifo_queue[i].pop();
    }
     
    std::ofstream outfile(file_name);
    while(!sorting_list.empty() && !GRACEFUL_EXIT) {
        // Sorts the vector based on 3rd character onwards
        sort(sorting_list.begin(), sorting_list.end(), sort_string);
        // gets the first element of the newly sorted vector
        std::string line = sorting_list.front();
        // writes the first element out to file
        outfile <<  line << "\n"; 
        // removes the element from the vector
        pop_front(sorting_list); 
        // checks if the queue the first element came from is empty
        if(!fifo_queue[line.length() - ARRAY_OFFSET].empty()){
            // If the queue is not empty get the next first element from that queue
            sorting_list.push_back(fifo_queue[line.length() - ARRAY_OFFSET].front());
            // Remove that first element from that queue
            fifo_queue[line.length() - ARRAY_OFFSET].pop();
        }
    }
    
    outfile.close();
    // Records the time after at the end of reduce4 and output the exectution time for reduce4.
    output_handler.print_log("## Reducing complete, single reduced filter files in 'Output/Task4' directory as '" 
        + file_name + "'");
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Reduce4",time_taken);
    return NULL; 
}
void *pthread_write(void *args) {
    // Starts the time for measuring execution time 
    // of mapping threads
    clock_t start, end;
    start = clock();
    struct MapParams *mapData = (struct MapParams *)args;
    std::string word_size = std::to_string(mapData->word_index + ARRAY_OFFSET);
    output_handler.print_log("Map Thread" + word_size + " Has Started Execution!");
    std::string fifo_name = "Task4Files/Fifo/FIFO" + word_size;

    // Open FIFO
    output_handler.print_log("Opening Fifo File " + fifo_name + " For Write!");
    int fd = open(fifo_name.c_str(), O_WRONLY);
    if(fd  == -1) { 
        output_handler.print_error("Fifo File " + fifo_name + " Has Failed to Open!");
        return NULL; 
    }

    // Sort the indexes of that word length
    sort(mapData->indexes.begin(), mapData->indexes.end(), sort_global); 

    // Write each of the index's for that length
     output_handler.print_log("Writing Strings out to " + fifo_name);
    for(int index = 0; index < (int)mapData->indexes.size() && !GRACEFUL_EXIT; index++) {
        if (write(fd, &mapData->indexes.at(index), sizeof(int)) == -1) {
            output_handler.print_error("Error writing to FIFO File");
            return NULL;
        };
    };
    output_handler.print_log("Writing Strings out to " + fifo_name + " Has Completed!");
    // Close the handle, return the thread
    close(fd); 
    output_handler.print_log("Closing Fifo File " + fifo_name + " For Write!");
    // Records the time after at the end of mapping thread and
    // output the exectution time for that thread.
    output_handler.print_log("Map Thread" + word_size + " Has Finished Execution!"); 
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Map Thread"+std::to_string(mapData->word_index + ARRAY_OFFSET),time_taken);
    return NULL;
}
void *pthread_read(void *args)  {
    // Starts the time for measuring execution time 
    // of individual reduce threads
    clock_t start, end;
    start = clock();
    // Gets the parameters of reduce threads from arguments
    struct ReduceParams *mapData = (struct ReduceParams *)args;
    // Gets the word length of current thread
    std::string word_size = std::to_string(mapData->index + ARRAY_OFFSET);
    output_handler.print_log("Reduce Thread" + word_size + " Has Started Execution!");
    std::string fifo_name = "Task4Files/Fifo/FIFO" + word_size;
    // Open FIFO
    output_handler.print_log("Opening Fifo File " + fifo_name + " For Read!");
    int fd = open(fifo_name.c_str(), O_RDONLY);
    if(fd  == -1) { 
        output_handler.print_error("Fifo File " + fifo_name + " Has Failed to Open!");
        return NULL; }

    int index; 
    // Read each of the index's for that length into the queue
    output_handler.print_log("Reading Strings out to " + fifo_name);
    while(read(fd , &index, sizeof(int)) > 0 && !GRACEFUL_EXIT) {
        mapData->stored_strings->push(Global.at(index));
    }
    output_handler.print_log("Reading Strings out to " + fifo_name + " Has Completed!");
    close(fd);

    // Records the time after at the end of reduce thread and
    // output the exectution time for that thread.
    output_handler.print_log("Closing Fifo File " + fifo_name + " For Read!");
    output_handler.print_log("Reduce Thread" + word_size + " Has Finished Execution!"); 
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Reduce Thread"+std::to_string(mapData->index + ARRAY_OFFSET),time_taken);
    return NULL;
}
bool sort_global(int index1, int index2)
{
   return Global[index1].substr(2,-1) < Global[index2].substr(2,-1);
}
void graceful_exit_handler(int seconds) {
    output_handler.print_error("Program running to long, " + std::to_string(GRACEFUL_SECONDS) + "s set as the limit, exiting");
    GRACEFUL_EXIT = true; 
}