#include <iostream>
#include "lame.h"
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <stdio.h>
#include <thread>
namespace fs = std::filesystem;


bool is_wav(const std::string& str) {
    // std::cout << str.compare(str.length()-4, 4, ".wav") << std::endl;
    return str.compare(str.length()-4, 4, ".wav") == 0;
}

std::shared_ptr<std::vector<std::string>> get_wav_files(std::string path) {
    auto vec = std::make_shared<std::vector<std::string> >();
    for (const auto & entry : fs::directory_iterator(path)) {
        const std::string path = entry.path();
        if (is_wav(path)) {
            vec->push_back(path);
        }
    }
    return vec;
}

std::string get_mp3_path(const std::string& str) {
    return str.substr(0, str.length()-4) + ".mp3";
}

void convert_to_mp3(lame_global_flags *gfp, std::string path) {
    int BUFSIZE = 20480;
    FILE *wav = fopen(path.data(), "rb");
    FILE *mp3 = fopen(get_mp3_path(path).data(), "wb");
    short int w_buf[BUFSIZE*2];
    unsigned char m_buf[BUFSIZE];

    fseek(wav, 24,SEEK_SET);
    int sample_rate = 0;
    fread(&sample_rate, 4, 1, wav);
    fseek(wav, 44, SEEK_SET);
    lame_set_in_samplerate(gfp, sample_rate);
    lame_init_params(gfp);

    int read = 0;
    int write = 0;
    do {
        read = fread(w_buf, 2*sizeof(short int), BUFSIZE, wav);
        if (read == 0) {
            write = lame_encode_flush(gfp, m_buf, BUFSIZE);
        } else {
            write = lame_encode_buffer_interleaved(gfp, w_buf, read, m_buf, BUFSIZE);
        }

        fwrite(m_buf, write, 1, mp3);
    } while (read != 0);

    fclose(wav);
    fclose(mp3);
    std::cout << "converted " + path << std::endl;
}

void convert_files_to_mp3(std::shared_ptr<std::vector<std::string> > vec, int start, int step) {
    lame_global_flags *gfp;
    gfp = lame_init();

    for (int i = start; i < vec->size(); i += step) {
        convert_to_mp3(gfp, vec->at(i));
    }

    lame_close(gfp);
}

int main(int argc, char** argv) {
    std::vector<std::string> params(argv, argv+argc);

    if (params.size() != 2) {
        std::cout << "Requires exactly one parameter. Path to folder." << std::endl;
        return -1;
    }

    std::string path = params[1];

    auto vec = get_wav_files(path);

    int cores = std::thread::hardware_concurrency();
    if (cores == 0) {
        std::cout << "Couldn't detect number of cores. Setting to 8." << std::endl;
        cores = 8;
    }
    std::vector<std::thread> threads;
    for (int i = 0; i < cores; i++) {
        threads.push_back(std::thread{convert_files_to_mp3, vec, i, cores});
    }

    // std::for_each(threads.begin(),threads.end(), std::mem_fn(&std::thread::join));
    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    std::cout << "All files converted." << std::endl;
}