f. First run repository.c, followed by writer.c, and then another instance of writer.c in another terminal. You'll see that 
the second writer is waiting for the first writer and writes only after completion of 1st writer.

g. First run repository.c, followed by writer.c and then run reader.c in another terminal. You'll see that the reader is 
waiting for the writer and reads only after completion of the writer.

h. First run repository.c, followed by reader.c and then another instance of reader.c in another terminal. You'll see that
both the readers read the data simultanoeously from the shared memory.