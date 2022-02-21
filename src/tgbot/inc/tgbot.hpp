#pragma once

#ifndef TGBOT_HEADER
#define TGBOT_HEADER

#include <tgbot/tgbot.h>
#include <string>
#include <iostream>
#include <map>

using std::string, std::cout, std::endl, std::map;

class TG_Bot
{
    private:
        /**
         * @brief Pointer to TgBot object
         */
        TgBot::Bot *bot;

        /**
         * @brief Long poll pointer 
         */
        TgBot::TgLongPoll *longPoll;

        /**
         * @brief Parameters for message
         */
        map<string, string> message_params;

        /**
         * @brief Callback for /start command
         * 
         * @param message Sended message
         */
        void command_start(TgBot::Message::Ptr message)
        {
            (*this->bot).getApi().sendMessage(message->chat->id, "Hi!");
        }

        void command_status(TgBot::Message::Ptr message)
        {

        }

    public:
        /**
         * @brief Construct a new tg bot object
         * 
         * @param token Telegram bot token
         */
        TG_Bot(
            const string& token
        )
        {
            this->bot = new TgBot::Bot(token);
        }

        /**
         * @brief Set the message params object
         * 
         * @param params Parameters for message
         */
        void set_message_params(map<string, string> &params) { this->message_params = params; }

        /**
         * @brief Initialize bot
         */
        void initialize()
        {
            (*this->bot).getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
                // this->command_start(message);
                (*this->bot).getApi().sendMessage(message->chat->id, "Hi!");
            });
            (*this->bot).getEvents().onCommand("status", [this](TgBot::Message::Ptr message) {
                string msg = "Status\n";
                for (auto& [key, val] : this->message_params)
                    msg = msg + key + " : " + val + '\n';
                (*this->bot).getApi().sendMessage(message->chat->id, msg);
            });

            this->longPoll = new TgBot::TgLongPoll(*this->bot);
        }

        /**
         * @brief Run the bot
         */
        void run()
        {
            while (true)
            {
                (*this->longPoll).start();
            }
        }

        /**
         * @brief Destroy the tg bot object
         */
        ~TG_Bot()
        {
            delete this->bot;
        }
};




























#endif