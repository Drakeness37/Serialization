#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define BENCHMARK_ADVANCED
#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>
#include "../data.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <json/json_features.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QFile>

using namespace rapidjson;
using json = nlohmann::json;

TEST_CASE("Nlohmann Json", "[benchmark]")
{
    const char* data = json_data::get_data();
    BENCHMARK_ADVANCED("To JSON from String")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data] { json::parse(data); });
    };

    json j = json::parse(data);
    BENCHMARK_ADVANCED("From JSON to String")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&j]() { j.dump(); });
    };

    j.clear();
    BENCHMARK_ADVANCED("FROM .json File to JSON object")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&j]() { std::ifstream i("../data.json"); i >> j; i.close(); });
        j.clear();
    };

    json jj = json::parse(data);
    BENCHMARK_ADVANCED("FROM JSON object to .json File")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&jj]() { std::ofstream o("../nlohmann_json.json"); o << std::setw(4) << jj << std::endl; o.close(); });
    };
    jj.clear();
}

TEST_CASE("RapidJson", "[benchmark]")
{
    const char* data = json_data::get_data();
    BENCHMARK_ADVANCED("To JSON from String")(Catch::Benchmark::Chronometer meter)
    {
        Document document;
        meter.measure([&document, &data] { document.Parse(data); });
    };

    Document document;
    document.Parse(data);
    BENCHMARK_ADVANCED("From JSON to String")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&document] {
            StringBuffer strbuf;
            strbuf.Clear();
            Writer<StringBuffer> writer(strbuf);
            document.Accept(writer);
            strbuf.GetString();
            strbuf.Clear();
        });
    };

    BENCHMARK_ADVANCED("FROM .json File to JSON object")(Catch::Benchmark::Chronometer meter)
    {
        Document d;
        meter.measure([&d]() { std::ifstream i("../data.json"); IStreamWrapper isw { i }; d.ParseStream(isw); i.close(); });
    };

    document.Parse(data);
    BENCHMARK_ADVANCED("FROM JSON object to .json File")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&document]() {
            std::ofstream ofs { "../rapidjson.json" };
            OStreamWrapper osw { ofs };
            Writer<OStreamWrapper> writer2 { osw };
            document.Accept( writer2 );
            ofs.close();
        });
    };

}

TEST_CASE("QJson", "[benchmark]")
{

    QString data = json_data::get_data();
    BENCHMARK_ADVANCED("To JSON from String")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&data] { QJsonDocument::fromJson(data.toUtf8()).object(); });
    };

    QJsonObject json = QJsonDocument::fromJson(data.toUtf8()).object();
    BENCHMARK_ADVANCED("From JSON to String")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&json] { QJsonDocument(json).toJson(QJsonDocument::Compact); });
    };

    BENCHMARK_ADVANCED("FROM .json File to JSON object")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([]() {
            QFile file;
            file.setFileName("../data.json");
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QJsonDocument::fromJson(file.readAll()).object();
            file.close();
        });
    };

    BENCHMARK_ADVANCED("FROM JSON object to .json File")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&json]() {
            QFile file("../qt.json");
            file.open(QIODevice::WriteOnly);
            QJsonDocument jsonDoc;
            jsonDoc.setObject(json);
            file.write(jsonDoc.toJson());
            file.close();
        });
    };
}
