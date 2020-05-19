#include "parse-jsoncpp.hpp"

using nlohmann::json;

namespace sensorversion {
    void to_json(json& j, const SensorVersion& sv) {
        j = json{{"dataType", sv.dataType}, 
                {"result", sv.result}, 
                {"master", sv.master},
                {"second", sv.second},
                {"step", sv.step}};
    }

    void from_json(const json& j, SensorVersion& sv) {
        sv.dataType = j.at("dataType").get<uint8_t>();
        sv.result = j.at("result").get<bool>();
        sv.master = j.at("master").get<uint8_t>();
        sv.second = j.at("second").get<uint8_t>();
        sv.step = j.at("step").get<uint8_t>();
    }
} 

namespace sensorstatus {
    void to_json(json& j, const SensorStatus& ss) {
        j = json{{"actlmode", ss.actl_mode}, 
                {"rollcount", ss.rollcount}, 
                {"cfgstatus", ss.cfgStatus}};
    }

    void from_json(const json& j, SensorStatus& ss) {
        ss.actl_mode = j.at("actlmode").get<uint8_t>();
        ss.rollcount = j.at("rollcount").get<uint8_t>();
        ss.cfgStatus = j.at("cfgstatus").get<uint8_t>();
    }
} 

namespace targetstatus {
    void to_json(json& j, const TargetStatus& ts) {
        j = json{{"nooftarget", ts.noOfTarget}, 
                {"rollcount", ts.rollcount}};
    }

    void from_json(const json& j, TargetStatus& ts) {
        ts.noOfTarget = j.at("nooftarget").get<uint8_t>();
        ts.rollcount = j.at("rollcount").get<uint8_t>();
    }
} 

namespace targetinfo {
    void to_json(json& j, const TargetInfo& ti) {
        j = json{{"index", ti.index}, 
                {"rcs", ti.rcs}, 
                {"range", ti.range},
                {"azimuth", ti.azimuth},
                {"vrel", ti.vrel},
				{"rollcount", ti.rollCount},
				{"snr", ti.SNR}};
    }

    void from_json(const json& j, TargetInfo& ti) {
        ti.index = j.at("index").get<uint8_t>();
        ti.rcs = j.at("rcs").get<float>();
        ti.range = j.at("range").get<float>();
        ti.azimuth = j.at("azimuth").get<int16_t>();
        ti.vrel = j.at("vrel").get<float>();
		ti.rollCount = j.at("rollcount").get<uint8_t>();
		ti.SNR = j.at("snr").get<int8_t>();
    }
} 

namespace sensordata {
	void to_json(json& j, const SensorData& sd) {
		sensorversion::to_json(j["sensorversion"], sd.version);
		sensorstatus::to_json(j["sensorstatus"], sd.sStatus);
		targetstatus::to_json(j["targetstatus"], sd.tStatus);
		targetinfo::to_json(j["targetinfo"], sd.tInfo);
		j["tinfosize"] = sd.tInfoSize;
	}

	void from_json(const json& j, SensorData& sd) {
		sensorversion::from_json(j.at("sensorversion"), sd.version);
		sensorstatus::from_json(j.at("sensorstatus"), sd.sStatus);
		targetstatus::from_json(j.at("targetstatus"), sd.tStatus);
		targetinfo::from_json(j.at("targetinfo"), sd.tInfo);
		sd.tInfoSize = j.at("tinfosize").get<int8_t>();
	}
}

/// @brief Serialize a SensorData object using jsoncpp.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
void jsoncpp_serialize_SensorData(SensorData input, json *out, enum option opt)
{
	if (opt == ARRAY) {
	} else {            
		sensordata::to_json(*out, input);
	}
}

/// @brief Deserialize a SensorData object using jsoncpp.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsoncpp_deserialize_SensorData(json& input, SensorData* output, enum option opt)
{
	if (opt == ARRAY) {
	} else {
        sensordata::from_json(input, *output);
	}
	return EXIT_SUCCESS;
}

// ---------- Serializer interface --------------------------------------------

/// @brief Initialize a context for JSON serialisation
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsoncpp_init(void* ctx)
{
	return EXIT_SUCCESS;
}

/// @brief Cleanup a context of JSON serialisation.
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsoncpp_cleanup(void* ctx)
{
	// Nothing to do
	return EXIT_SUCCESS;
}

/// @brief Print a string representation of the serialized data to the console.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to print.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsoncpp_print(void* ctx, void* data)
{
	if (!data) return EXIT_FAILURE;
    std::cout << ((*(json *)data).dump()) << std::endl;
	return EXIT_SUCCESS;
}

/// @brief Free the allocated serialized object.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to free.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsoncpp_freeobject(void* ctx, void* data)
{
	if (!(*(json *)data).empty()){
		(*(json *)data).clear();
		//free(data);
	}
	return !(*(json *)data).empty();
}

/// @brief Serialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Object to serialize.
/// @param[out] output Serialization result if any.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsoncpp_serialize(void* ctx, SensorData input, void** output)
{
	if (!output) return EXIT_FAILURE;
	enum option opt = *((option *)ctx);
	jsoncpp_serialize_SensorData(input,(json *)output, opt);
	return EXIT_SUCCESS;
}


/// @brief Deserialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Data to deserialize.
/// @param[out] output Deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsoncpp_deserialize(void* ctx, void* input, SensorData* output)
{
	if (!input || !output) return EXIT_FAILURE;
	enum option opt = *((option *)ctx);
	jsoncpp_deserialize_SensorData(*(json *)input, output, opt);
	return EXIT_SUCCESS;
}

void ParseJsonCpp::jsoncpp_get_serializer(Serializer* s)
{
	if (s)
	{
		s->context		= NULL;
		s->init			= jsoncpp_init;
		s->cleanup		= jsoncpp_cleanup;
		s->print		= jsoncpp_print;
		s->freeobject	= jsoncpp_freeobject;
		s->serialize	= jsoncpp_serialize;
		s->deserialize	= jsoncpp_deserialize;
	}
}