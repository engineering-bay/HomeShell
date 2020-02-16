#define DEFAULT_STRING_SIZE     256
#define SHORT_STRING_SIZE       32
#define LOG_RECTYPE_CONSOLE     1
#define LOG_RECTYPE_FILE        2
#define LOG_RECTYPE_SPEC        3
#define LOG_RECOPTS_NORMAL      0
#define LOG_RECOPTS_NOMILLIS    1
#define LOG_RECOPTS_NOSECONDS   2
#define SCHEDULE_DIVIDERS       5


// Write log message to console or file
void WriteToLog(int recordType, char *recMessage, int options);
// Executes scheduled tasks
int SchedulerCheck(struct tm* ptm);