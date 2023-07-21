constexpr unsigned int DEFAULT_CONCURRENCY = 6;

// One thread for reading words and putting them into queues
// Divide letter for queues with letter frequency weight
// For dividing words hash_table with {letter: link to queue}
// Create processign threads that do:
//      1. Check if queue empty
//      2. Take words from queue and process them
//      3. If queue empty check flag end
//      4. Return number of unique words