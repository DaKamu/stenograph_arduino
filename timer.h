
// milliseconds works just like millis, but we have re-implemented
// it as a project requirement.
//
// This function is not as precise as millis, so use it
// with caution. i.e. although `millis() % 1000 == 0`
// will work with millis, it will not work with this function
// until several seconds have passed (12 to be precise).
// 
// 1 seconds have elapsed && milliseconds = 1038
// 2 seconds have elapsed && milliseconds = 2035
// 3 seconds have elapsed && milliseconds = 3030
// 4 seconds have elapsed && milliseconds = 4027
// 5 seconds have elapsed && milliseconds = 5022
// 6 seconds have elapsed && milliseconds = 6019
// 7 seconds have elapsed && milliseconds = 7014
// 8 seconds have elapsed && milliseconds = 8011
// 9 seconds have elapsed && milliseconds = 9007
// 10 seconds have elapsed && milliseconds = 10005
// 11 seconds have elapsed && milliseconds = 11001
// 12 seconds have elapsed && milliseconds = 12000
// 13 seconds have elapsed && milliseconds = 13000
// ... works from here on out
//
// If you are counting seconds, it is recommended to convert
// the miliseconds into seconds and track whether you have
// used that second yet.
// 
unsigned long milliseconds();

// init the interrupts we use for milliseconds
void setup_timer_interrupts();
