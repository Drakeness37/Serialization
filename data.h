#ifndef JSON_LABA_DATA_H
#define JSON_LABA_DATA_H

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <fstream>

namespace json_data{
    const char* get_data() {
        rapidjson::Document tmp;
        std::ifstream i("../data.json");
        rapidjson::IStreamWrapper isw { i };
        tmp.ParseStream(isw);
        i.close();
        rapidjson::StringBuffer strbuf;
        strbuf.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        tmp.Accept(writer);
        return strbuf.GetString();
    }
}

#endif //JSON_LABA_DATA_H
