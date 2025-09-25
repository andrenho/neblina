#ifndef CONFIG_GEN_C_H__
#define CONFIG_GEN_C_H__

#include "sstr.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convert (marshal) array of int to indented json string.
 * @param obj The array of ints.
 * @param len length of array obj.
 * @indent The indent space of json string.
 * @curindent The current indent space of json string before calling this function, 
 * set to 0 if you don't kown what it means.
 * @param out The output json string.
 */
int json_marshal_array_indent_int(int* obj, int len, int indent, int curindent, sstr_t out);
/**
 * @brief Convert (marshal) array of longs to indented json string.
 * @param obj The array of longs.
 * @param len length of array obj.
 * @indent The indent space of json string.
 * @curindent The current indent space of json string before calling this function, 
 * set to 0 if you don't kown what it means.
 * @param out The output json string.
 */
int json_marshal_array_indent_long(long* obj, int len, int indent, int curindent, sstr_t out);
/**
 * @brief Convert (marshal) array of floats to indented json string.
 * @param obj The array of floats.
 * @param len length of array obj.
 * @indent The indent space of json string.
 * @curindent The current indent space of json string before calling this function, 
 * set to 0 if you don't kown what it means.
 * @param out The output json string.
 */
int json_marshal_array_indent_float(float* obj, int len, int indent, int curindent, sstr_t out);
/**
 * @brief Convert (marshal) array of double to indented json string.
 * @param obj The array of doubles.
 * @param len length of array obj.
 * @indent The indent space of json string.
 * @curindent The current indent space of json string before calling this function, 
 * set to 0 if you don't kown what it means.
 * @param out The output json string.
 */
int json_marshal_array_indent_double(double* obj, int len, int indent, int curindent, sstr_t out);
/**
 * @brief Convert (marshal) array of sstr_t to indented json string.
 * @param obj The array of sstr_t's.
 * @param len length of array obj.
 * @indent The indent space of json string.
 * @curindent The current indent space of json string before calling this function, 
 * set to 0 if you don't kown what it means.
 * @param out The output json string.
 */
int json_marshal_array_indent_sstr_t(sstr_t* obj, int len, int indent, int curindent, sstr_t out);

#define json_marshal_array_int(obj, len, out) json_marshal_array_indent_int(obj, len, 0, 0, out)
#define json_marshal_array_long(obj, len, out) json_marshal_array_indent_long(obj, len, 0, 0, out)
#define json_marshal_array_float(obj, len, out) json_marshal_array_indent_float(obj, len, 0, 0, out)
#define json_marshal_array_double(obj, len, out) json_marshal_array_indent_double(obj, len, 0, 0, out)
#define json_marshal_array_sstr_t(obj, len, out) json_marshal_array_indent_sstr_t(obj, len, 0, 0, out)

/**
 * @brief Convert (unmarshal) json string to array of int.
 * @param content The json string.
 * @param ptr A pointer to array of int.
 * @param len A pointer to int variable to store length of array.
 */
int json_unmarshal_array_int(sstr_t content, int** ptr, int* len);
/**
 * @brief Convert (unmarshal) json string to array of long.
 * @param content The json string.
 * @param ptr A pointer to array of long.
 * @param len A pointer to int variable to store length of array.
 */
int json_unmarshal_array_long(sstr_t content, long** ptr, int* len);
/**
 * @brief Convert (unmarshal) json string to array of double.
 * @param content The json string.
 * @param ptr A pointer to array of double.
 * @param len A pointer to int variable to store length of array.
 */
int json_unmarshal_array_double(sstr_t content, double** ptr, int* len);
/**
 * @brief Convert (unmarshal) json string to array of float.
 * @param content The json string.
 * @param ptr A pointer to array of floats.
 * @param len A pointer to int variable to store length of array.
 */
int json_unmarshal_array_float(sstr_t content, float** ptr, int* len);
/**
 * @brief Convert (unmarshal) json string to array of sstr_t.
 * @param content The json string.
 * @param ptr A pointer to array of sstr_t.
 * @param len A pointer to int variable to store length of array.
 */
int json_unmarshal_array_sstr_t(sstr_t content, sstr_t** ptr, int* len);

struct Service {
    sstr_t name;
    int active;
    int port;
    int openToWorld;
    int loggingColor;
};

/**
 * @brief initialize function for struct Service
 * it set all fields of obj to 0.
 *
 * @param obj the struct object to be initialized
 */
int Service_init(struct Service* obj);
/**
 * @brief uninitialize function for struct Service
 * it set all fields of obj to 0, and free all
 * dynamically allocated memory of fields inside recursively.
 */
int Service_clear(struct Service* obj);
/**
 * @brief Convert (marshal) struct Service to a well indented json string.
 * @param obj the struct object to be marshaled
 * @param indent the indentation spaces of the output json string
 * @param curindent the current indentation before call this function,
 * set it to 0 if for normal purpose.
 * @param out the output json string.
 */
int json_marshal_indent_Service(struct Service* obj, int indent, int curindent, sstr_t out);
/**
 * @brief Convert (marshal) struct Service to a not indented json string.
 * @param obj the struct object to be marshaled
 * @param out the output json string.
 */
#define json_marshal_Service(obj, out) json_marshal_indent_Service(obj, 0, 0, out)
/**
 * @brief Convert (marshal) an array of struct Service to a well indented json string.
 * @param obj the array of struct object to be marshaled
 * @param indent the indentation spaces of the output json string
 * @param curindent the current indentation before call this function,
 * set it to 0 if for normal purpose.
 * @param out the output json string.
 */
int json_marshal_array_indent_Service(struct Service* obj, int len, int indent, int curindent, sstr_t out);
/**
 * @brief Convert (marshal) array of struct Service to a (un)indented json string.
 * @param obj the struct object to be marshaled
 * @param out the output json string.
 */
#define json_marshal_array_Service(obj, len, out) json_marshal_array_indent_Service(obj, len, 0, 0, out)
/**
 * @brief Convert (unmarshal) a json string to an object of struct Service.
 * @param in the input json string.
 * @param obj the output struct object.
 */
int json_unmarshal_Service(sstr_t in, struct Service* obj);
/**
 * @brief Convert (unmarshal) a json string to an object array of struct Service.
 * @param in the input json string.
 * @param obj the output struct object.
 * @param len the output length of the array.
 */
int json_unmarshal_array_Service(sstr_t in, struct Service** obj, int* len);

struct Config {
    struct Service* services;
    int services_len;
};

/**
 * @brief initialize function for struct Config
 * it set all fields of obj to 0.
 *
 * @param obj the struct object to be initialized
 */
int Config_init(struct Config* obj);
/**
 * @brief uninitialize function for struct Config
 * it set all fields of obj to 0, and free all
 * dynamically allocated memory of fields inside recursively.
 */
int Config_clear(struct Config* obj);
/**
 * @brief Convert (marshal) struct Config to a well indented json string.
 * @param obj the struct object to be marshaled
 * @param indent the indentation spaces of the output json string
 * @param curindent the current indentation before call this function,
 * set it to 0 if for normal purpose.
 * @param out the output json string.
 */
int json_marshal_indent_Config(struct Config* obj, int indent, int curindent, sstr_t out);
/**
 * @brief Convert (marshal) struct Config to a not indented json string.
 * @param obj the struct object to be marshaled
 * @param out the output json string.
 */
#define json_marshal_Config(obj, out) json_marshal_indent_Config(obj, 0, 0, out)
/**
 * @brief Convert (marshal) an array of struct Config to a well indented json string.
 * @param obj the array of struct object to be marshaled
 * @param indent the indentation spaces of the output json string
 * @param curindent the current indentation before call this function,
 * set it to 0 if for normal purpose.
 * @param out the output json string.
 */
int json_marshal_array_indent_Config(struct Config* obj, int len, int indent, int curindent, sstr_t out);
/**
 * @brief Convert (marshal) array of struct Config to a (un)indented json string.
 * @param obj the struct object to be marshaled
 * @param out the output json string.
 */
#define json_marshal_array_Config(obj, len, out) json_marshal_array_indent_Config(obj, len, 0, 0, out)
/**
 * @brief Convert (unmarshal) a json string to an object of struct Config.
 * @param in the input json string.
 * @param obj the output struct object.
 */
int json_unmarshal_Config(sstr_t in, struct Config* obj);
/**
 * @brief Convert (unmarshal) a json string to an object array of struct Config.
 * @param in the input json string.
 * @param obj the output struct object.
 * @param len the output length of the array.
 */
int json_unmarshal_array_Config(sstr_t in, struct Config** obj, int* len);


#ifdef __cplusplus
}
#endif

#endif

