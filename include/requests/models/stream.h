#ifndef CPPREQUESTS_STREAM_H
#define CPPREQUESTS_STREAM_H

#include <cstring>

namespace crq {



    class StreamBuffer {

    private:
        Request &_handler;

        HeaderMap header_buffer;
        std::string body_buffer;

        bool pass_header = false;
        bool _consumed = false;

    public:
        explicit StreamBuffer(Request &handler) : _handler(handler) {
            this->body_buffer.reserve(1024);
        }

        static std::size_t write_callback(void *contents,
                                          std::size_t size,
                                          std::size_t nmemb,
                                          void *userdata) {
            // 转换缓冲区指针为StreamBuffer*
            auto buffer = (StreamBuffer*)(userdata);

            // 计算缓冲区内数据长度
            const std::size_t content_length = size * nmemb;

            // 取得缓冲区头指针
            const auto char_contents = (char*) contents;

            // 如果还未经过header分隔符
            if (!buffer->pass_header) {
                if ((char_contents[0] == '\r') && (char_contents[1] == '\n')) {
                    buffer->pass_header = true;
                }
                return content_length;
            }

            // 将数据追加至缓冲区内
            buffer->body_buffer.append(char_contents, content_length);
            return content_length;
        }

        static std::size_t header_callback(void *contents,
                                           std::size_t size,
                                           std::size_t nmemb,
                                           void *userdata) {

            const std::size_t content_length = size * nmemb;

            auto convert_userdata = (HeaderMap*)(userdata);

            auto char_content = (char*) contents;

            if ((char_content[0] == '\r') && (char_content[1] == '\n')) {
                return content_length;
            }

            const auto split_pos = strchr(char_content, ':');

            if (split_pos != nullptr) {
                const char *key_head, *key_end;
                std::tie(key_head, key_end) = string::char_strip(char_content, split_pos);

                const char *value_head, *value_end;
                std::tie(value_head, value_end) = string::char_strip(split_pos + 1, char_content + content_length);

                const auto key = std::string(key_head, key_end);
                const auto value = std::string(value_head, value_end);

                convert_userdata->operator[](key) = value;
            }

            return content_length;
        }

        REF_PROPERTY(header, header_buffer, HeaderMap);

        REF_PROPERTY(body, body_buffer, std::string);

        PRIMARY_PROPERTY(consumed, _consumed, bool);
    };
}

#endif //CPPREQUESTS_STREAM_H
