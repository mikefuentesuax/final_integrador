#include <vector>
#include <string>
#include <map>
#include "json11.hpp" // Asegúrate de tener esta biblioteca en tu sistema
#include "jsonlib.hpp" // No estoy seguro de qué biblioteca es esta, asegúrate de tenerla en tu sistema

enum variant_type { Symbol, Number, List, Proc, Lambda, Cadena };

struct Entorno;

class Variant {
public:
    typedef Variant(*proc_type) ( const std::vector<Variant>& );
    typedef std::vector<Variant>::const_iterator iter;
    typedef std::map<std::string, Variant> map;

    variant_type type;
    std::string val;
    std::vector<Variant> list;
    proc_type proc;
    Entorno* env;

    Variant(variant_type type = Symbol) : type(type) , env(0), proc(0) { }
    Variant(variant_type type, const std::string& val) : type(type), val(val) , env(0) , proc(0) { }
    Variant(proc_type proc) : type(Proc), proc(proc) , env(0) { }

    std::string to_string();
    std::string to_json_string();
    static Variant from_json_string(std::string json);
    static Variant parse_json(jsonlib::Json job);  // Asegúrate de que esta es la definición correcta de la función
};

std::string Variant::to_string() {
    switch (type) {
        case Symbol: return val;
        case Number: return val;
        case List: {
            std::string result = "[";
            for (const auto& item : list) {
                result += item.to_string() + ", ";
            }
            if (!list.empty()) result.pop_back(); // Elimina la coma final
            result += "]";
            return result;
        }
        case Proc: return "Proc";
        case Lambda: return "Lambda";
        case Cadena: return val;
        default: return "";
    }
}

std::string Variant::to_json_string() {
    json11::Json::object json_obj;
    switch (type) {
        case Symbol:
            json_obj["type"] = "Symbol";
            json_obj["value"] = val;
            break;
        case Number:
            json_obj["type"] = "Number";
            json_obj["value"] = val;
            break;
        case List: {
            json_obj["type"] = "List";
            json_obj["value"] = json11::Json::array(list.begin(), list.end());
            break;
        }
        case Proc:
            json_obj["type"] = "Proc";
            break;
        case Lambda:
            json_obj["type"] = "Lambda";
            break;
        case Cadena:
            json_obj["type"] = "Cadena";
            json_obj["value"] = val;
            break;
        default:
            break;
    }
    return json11::Json(json_obj).dump();
}

Variant Variant::from_json_string(std::string sjson) {
    std::string err;
    json11::Json json_obj = json11::Json::parse(sjson, err);
    if (!err.empty()) {
        // Manejar el error de parsing según tus necesidades
        return Variant();
    }

    std::string type = json_obj["type"].string_value();
    if (type == "Symbol") {
        return Variant(Symbol, json_obj["value"].string_value());
    } else if (type == "Number") {
        return Variant(Number, json_obj["value"].string_value());
    } else if (type == "List") {
        Variant result(List);
        for (const auto& item : json_obj["value"].array_items()) {
            result.list.push_back(parse_json(item));
        }
        return result;
    } else if (type == "Proc") {
        return Variant([](const std::vector<Variant>&) { return Variant(); });
    } else if (type == "Lambda") {
        return Variant(Lambda);
    } else if (type == "Cadena") {
        return Variant(Cadena, json_obj["value"].string_value());
    } else {
        // Manejar el caso de tipo desconocido según tus necesidades
        return Variant();
    }
}

Variant Variant::parse_json(jsonlib::Json job) {
    // Implementa el parseo de JSON según tus necesidades y la estructura de jsonlib::Json
    return Variant();
}