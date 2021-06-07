#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdarg.h>

#include "tsc_x86.h"

namespace timings {
    long start;
    std::vector<std::string> lines;
    long total;
    std::string sample_name;
    int counter;


    void init_timer(const char* sample_name) {
        timings::sample_name = sample_name;
        timings::total = 0;
        timings::counter = 0;
        timings::start = start_tsc();
    }

    void record_timing(const char* tag) {
        long cycles = stop_tsc(timings::start);
        timings::total += cycles;
        std::string line = timings::sample_name + ":" + tag
            + "," + timings::sample_name
            + "," + tag
            + "," + std::to_string(timings::counter++)
            + "," + std::to_string(cycles)
            + "," + std::to_string(timings::total)
        ;
        timings::lines.push_back(line);
        timings::start = start_tsc();
    }

    void record_timing_f(const char* format, ...) {
        long cycles = stop_tsc(timings::start);
        timings::total += cycles;
        char tag[100];
        va_list args;
        va_start(args, format);
        vsprintf(tag, format, args);
        std::string line = timings::sample_name + ":" + tag
            + "," + timings::sample_name
            + "," + tag
            + "," + std::to_string(timings::counter++)
            + "," + std::to_string(cycles)
            + "," + std::to_string(timings::total)
        ;
        timings::lines.push_back(line);
        timings::start = start_tsc();
    }

    void timer_skip_to_here() {
        timings::start = start_tsc();
    }

    void record_timing_custom(const char* tag, long time, long total) {
        stop_tsc(timings::start);
        std::string line = timings::sample_name + ":" +tag
            + "," + timings::sample_name
            + "," + tag
            + "," + std::to_string(timings::counter++)
            + "," + std::to_string(time)
            + "," + std::to_string(total)
        ;
        start_tsc();
    }

    void write_out_timings(const char* filename) {
        std::ofstream file(filename);
        file << "name+tag,name,tag,counter,cycles,total" << std::endl;
        for(std::string line : timings::lines) {
            file << line << std::endl;
        }
        file.close();
    }

}