#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib> 

using json = nlohmann::json;

const std::string BOT_TOKEN = "YOUR_BOT_TOKEN"; // Your bot token
const std::string CHANNEL_ID = "YOUR_CHANNEL_ID"; // Your channel ID
const std::string BOT_ID = "YOUR_BOT_ID"; // Your bot's user ID

std::vector<std::string> replies = {
    "tere maa ka bhoda mara jaake idc about your {}",
    "Teri behen ki tight chut ko laoak lapak kar speed se chod du {}",
    "Tera baap abhi mere lode ko chaat chaat kar khus hota hai {}",
    "Teri behen gand me kutub minaar {}",
    "Teri behen hijdi what ae ranidi family {}",
    "Tere maa ki chudai mein main raat bhar lode dalta rahunga {}",
    "Tere baap gand mara gaya {}",
    "Sun ra chudi hui randi ke aulad, kya bola tera behen aaya bhai tu kya kar raha hai {}",
    "Tere maa ko koi na jode to kya hoga {}",
    "Kya bhai tujhe roti mil gayi kya tere behen ki chut se {}",
    "Teri behen ka chudai pura maidan bana diya main ne {}",
    "Tere ko kya samajh hai, tere behen ka chupke kya chudai hota hai {}",
    "Tere bhai ne suni hai baat, main aa raha hoon {}"
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void sendMessage(const std::string& message) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    json payload = {
        {"content", message},
        {"allowed_mentions", {
            {"parse", {"users"}}  // Ensures the user is pinged
        }}
    };

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bot " + BOT_TOKEN).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, ("https://discord.com/api/v9/channels/" + CHANNEL_ID + "/messages").c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.dump().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Disable SSL certificate verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}

void listenForMessages() {
    while (true) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, ("https://discord.com/api/v9/channels/" + CHANNEL_ID + "/messages?limit=1").c_str());
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, ("Authorization: Bot " + BOT_TOKEN).c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            // Disable SSL certificate verification
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

            res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                try {
                    json response = json::parse(readBuffer);

                    // Check if the response is an array and has at least one message
                    if (response.is_array() && !response.empty()) {
                        std::string username = response[0]["author"]["username"];
                        std::string messageContent = response[0]["content"];
                        std::string authorID = response[0]["author"]["id"];

                        // Don't reply to itself
                        if (authorID == BOT_ID) {
                            std::cout << "Ignoring bot's own message..." << std::endl;
                            continue;
                        }

                        // Prepare the message with the ping and roast
                        std::string reply = replies[rand() % replies.size()];
                        reply.replace(reply.find("{}"), 2, "<@" + authorID + ">: \"" + messageContent + "\"");
                        sendMessage(reply);
                    }
                } catch (const json::exception& e) {
                    std::cerr << "JSON error: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            }

            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
        }

        std::this_thread::sleep_for(std::chrono::seconds(5)); // Check for new messages every 5 seconds
    }
}

int main() {
    std::cout << "Starting Discord bot..." << std::endl;
    std::srand(std::time(0)); // Seed for random number generation
    listenForMessages();
    return 0;
}

// Compile command
// g++ AI2.cpp -o AI2 -I"C:/curl-8.10.1_6-win64-mingw/curl-8.10.1_6-win64-mingw/include" -L"C:/curl-8.10.1_6-win64-mingw/curl-8.10.1_6-win64-mingw/lib" -lcurl -I"C:/json-develop/json-develop/include" -I"C:/json-develop/json-develop/include/nlohmann"
