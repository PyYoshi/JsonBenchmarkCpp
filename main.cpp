/*
 * JsonBenchmarkCpp
 * A small program to compare perfomance of different json libs available
 *
 * Currently supporting following libs,
 *
 * 1. Cajun
 * 2. json_spirit
 * 3. libjson
 * 4. json-parser
 *
 * Copyright Lijo Antony 2011
 * Distributed under Apache License, Version 2.0
 * see accompanying file LICENSE.txt
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <time.h>

#ifdef __MACH__
  #include <mach/clock.h>
  #include <mach/mach.h>
  #define CLOCK_ID_T clock_id_t
  #define CLOCK_MONOTONIC SYSTEM_CLOCK
  #define CLOCK_REALTIME CALENDAR_CLOCK
  #define CLOCK_PROCESS_CPUTIME_ID SYSTEM_CLOCK
  #define CLOCK_THREAD_CPUTIME_ID SYSTEM_CLOCK
#else
  #define CLOCK_ID_T clockid_t
#endif

//Cajun headers
#include <json/reader.h>
#include <json/writer.h>
#include <json/elements.h>

//json_spirit headers
#include <json_spirit.h>

//libjson headers
#include <libjson.h>

//json-parser headers
#define json_string __json_string
#include <json.h>
#undef json_string

//json11 headers
#include <json11.hpp>

// picojson headers
#include <picojson.h>

// compat osx
void time_(CLOCK_ID_T clock, struct timespec* t)
{
  #ifdef __MACH__
    // OS X does not have clock_gettime, use clock_get_time
    // see http://stackoverflow.com/questions/11680461/monotonic-clock-on-osx
    clock_serv_t cclock;
    mach_timespec_t mts;
    struct timespec* ts;
    host_get_clock_service(mach_host_self(), clock, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    t->tv_sec = mts.tv_sec;
    t->tv_nsec = mts.tv_nsec;
  #else
    clock_gettime(clock, (struct timespec*)t);
  #endif
}

/*
 * @brief A function to print time duration
 *
 * @param start starttime
 * @param end   endtime
 * @return none
 */
void printTimeDiff(timespec start, timespec end)
{
    timespec temp;

    if (end.tv_nsec > start.tv_nsec)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }

    unsigned long long int usecs = (unsigned long long int)(temp.tv_sec) * 1000000 + (unsigned long long int)(temp.tv_nsec / 1000);

    std::cout << std::setw(25) << std::left << usecs;
}

/*
 * @brief function for cajun benchmark
 *
 * @param jsonString test data as a string
 * @return none
 */
void cajunBenchmark(std::string jsonString)
{
    std::istringstream buff(jsonString);
    timespec time1, time2;
    json::Object obj;

    std::cout << std::setw(25) << "cajun";

    //Parsing the string
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    json::Reader::Read(obj, buff);
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);

    //Serialize to string
    std::ostringstream out;
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    json::Writer::Write(obj, out);
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);
    std::cout << std::endl;
}

/*
 * @brief function for json_spirit benchmark
 *
 * @param jsonString test data as a string
 * @return none
 */
void jsonspiritBenchmark(std::string jsonString)
{
    std::istringstream buff(jsonString);
    timespec time1, time2;
    json_spirit::mValue value;

    std::cout << std::setw(25) << "json_spirit";

    //Parsing the string
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    json_spirit::read( buff, value );
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);

    //Serialize to string
    std::ostringstream out;
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    json_spirit::write(value, out);
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);
    std::cout << std::endl;
}
/*
 * @brief function for libjson benchmark
 *
 * @param jsonString test data as a string
 * @return none
 */
void libjsonBenchmark(std::string jsonString)
{
    timespec time1, time2;

    std::cout << std::setw(25) << "libjson";

    //Parsing the string
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    JSONNode n = libjson::parse(jsonString);
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);

    //Serialize to string
    std::string out;
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    out = n.write();
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);
    std::cout << std::endl;
}

/*
 * @brief function for json-parser benchmark
 *
 * @param jsonString test data as a string
 * @return none
 */
void jsonparserBenchmark(std::string jsonString)
{
    timespec time1, time2;
    json_value * value;

    std::cout << std::setw(25) << "json_parser";

    //Parsing the string
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    value = json_parse(jsonString.c_str());
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);

    std::cout << std::endl;
}

void json11Benchmark(std::string jsonString) {
    timespec time1, time2;
    std::string err;

    std::cout << std::setw(25) << "json11";

    //Parsing the string
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    json11::Json jo = json11::Json::parse(jsonString, err);
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);

    //Serialize to string
    std::string out;
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    out = jo.dump();
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);
    std::cout << std::endl;
}

void picojsonBenchmark(std::string jsonString) {
    timespec time1, time2;
    picojson::value val;
    std::string err;

    std::cout << std::setw(25) << "picojson";

    //Parsing the string
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    picojson::parse(val, jsonString.begin(), jsonString.end(), &err);
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);

    //Serialize to string
    time_(CLOCK_PROCESS_CPUTIME_ID, &time1);
    val.serialize();
    time_(CLOCK_PROCESS_CPUTIME_ID, &time2);

    printTimeDiff(time1, time2);
    std::cout << std::endl;
}

int main()
{

    std::ifstream ifs("data.json", std::ifstream::in);
    std::string buff = "";

    if(ifs.is_open())
    {
        while(!ifs.eof())
        {
            std::string temp;
            ifs >> temp;
            buff += temp;
        }
    }

    if(buff.empty())
    {
        std::cout << "No data available for test, exiting!" << std::endl;
        exit(1);
    }

    std::cout << std::setw(25) << std::left << "#library"
              << std::setw(25) << std::left << "parsing"
              << std::setw(25) << std::left << "writing"
              << std::endl;

    cajunBenchmark(buff);
    jsonspiritBenchmark(buff);
    libjsonBenchmark(buff);
    jsonparserBenchmark(buff);
    json11Benchmark(buff);
    picojsonBenchmark(buff);

    return 0;
}
