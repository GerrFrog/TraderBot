#pragma once

#ifndef EXCEPTIONS_HEADER
#define EXCEPTIONS_HEADER

#include <stdexcept>
#include <exception>
#include <typeinfo>

// TODO: Bad config exception

/**
 * @brief Panic Exceptions
 */
namespace Exceptions::Panic
{
    /**
     * @brief Something terrible happened
     */
    class Panic_Exception: virtual public std::exception {
        protected:
            /**
             * @brief Error number
             */
            int error_number;

            /**
             * @brief Error offset
             */
            int error_offset;

            /**
             * @brief Error message
             */
            std::string error_message;
            
        public:
            /** 
             * @brief Constructor 
             * 
             * @param msg The error message
             * @param err_num Error number
             * @param err_off Error offset
             */
            explicit Panic_Exception(
                const std::string& msg, 
                int err_num, 
                int err_off
            ) : error_number(err_num),
                error_offset(err_off),
                error_message(msg) { }

            /**
             * @brief Destroy the Panic_exception object
             */
            virtual ~Panic_Exception() throw () { }

            /** 
             * @brief Returns a pointer to the (constant) error description.
             * 
             * @return char* A pointer to a const char*. The underlying memory
             * is in possession of the Except object. Callers must
             * not attempt to free the memory.
             */
            virtual const char* what() const throw () 
            {
                return error_message.c_str();
            }
            
            /**
             * @brief Get the Error Number 
             * 
             * @return int Error number
             */
            virtual int getErrorNumber() const throw() 
            {
                return error_number;
            }
            
            /**
             * @brief Get the Error Offset 
             * 
             * @return int Error ofset
             */
            virtual int getErrorOffset() const throw() 
            {
                return error_offset;
            }
    };
}

/**
 * @brief Exceptions for strategies
 */
namespace Exceptions::Strategies
{
    /**
     * @brief Error in Strategy logic
     */
    class Logic_Exception: virtual public std::exception {
        protected:
            /**
             * @brief Error number
             */
            int error_number;

            /**
             * @brief Error offset
             */
            int error_offset;

            /**
             * @brief Error message
             */
            std::string error_message;
            
        public:
            /** 
             * @brief Constructor 
             * 
             * @param msg The error message
             * @param err_num Error number
             * @param err_off Error offset
             */
            explicit Logic_Exception(
                const std::string& msg, 
                int err_num, 
                int err_off
            ) : error_number(err_num),
                error_offset(err_off),
                error_message(msg) { }

            /**
             * @brief Destroy the Logic_Exception object
             */
            virtual ~Logic_Exception() throw () { }

            /** 
             * @brief Returns a pointer to the (constant) error description.
             * 
             * @return char* A pointer to a const char*. The underlying memory
             * is in possession of the Except object. Callers must
             * not attempt to free the memory.
             */
            virtual const char* what() const throw () 
            {
                return error_message.c_str();
            }
            
            /**
             * @brief Get the Error Number 
             * 
             * @return int Error number
             */
            virtual int getErrorNumber() const throw() 
            {
                return error_number;
            }
            
            /**
             * @brief Get the Error Offset 
             * 
             * @return int Error ofset
             */
            virtual int getErrorOffset() const throw() 
            {
                return error_offset;
            }
    };
}

/**
 * @brief Exceptions for requests to taapi.io
 */
namespace Exceptions::TAAPI
{
    /**
     * @brief Request limit to taapi.io
     */
    class Rate_Limit: virtual public std::exception {
        protected:
            /**
             * @brief Error number
             */
            int error_number;

            /**
             * @brief Error offset
             */
            int error_offset;

            /**
             * @brief Error message
             */
            std::string error_message;
            
        public:
            /** 
             * @brief Constructor 
             * 
             * @param msg The error message
             * @param err_num Error number
             * @param err_off Error offset
             */
            explicit Rate_Limit(
                const std::string& msg, 
                int err_num, 
                int err_off
            ) : error_number(err_num),
                error_offset(err_off),
                error_message(msg) { }

            /**
             * @brief Destroy the Rate_Limit object
             */
            virtual ~Rate_Limit() throw () { }

            /** 
             * @brief Returns a pointer to the (constant) error description.
             * 
             * @return char* A pointer to a const char*. The underlying memory
             * is in possession of the Except object. Callers must
             * not attempt to free the memory.
             */
            virtual const char* what() const throw () 
            {
                return error_message.c_str();
            }
            
            /**
             * @brief Get the Error Number 
             * 
             * @return int Error number
             */
            virtual int getErrorNumber() const throw() 
            {
                return error_number;
            }
            
            /**
             * @brief Get the Error Offset 
             * 
             * @return int Error ofset
             */
            virtual int getErrorOffset() const throw() 
            {
                return error_offset;
            }
    };
}













#endif