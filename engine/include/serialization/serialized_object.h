#pragma once

#include <cstdlib>
#include <ostream>
#include <sstream>
#include <vector>

#include "templating/is_base_of_template.h"

namespace nebula {

    template <typename T>
    class Serializable;

    template <typename T>
    class SerializedObject {

    public:
        struct Meta {
            vector<uint8_t> id{};
        };

        explicit SerializedObject() {
            stringstream s;
            s << T::__EXT << T::__VER;
            string str(s.rdbuf()->str());
            _meta.id = vector<uint8_t>(str.begin(), str.end());
        }

        void clear() {
            _body.clear();
            _body.shrink_to_fit();
        }

        void addId() {
            *this << _meta.id;
        }

        void removeId() {
            *this >> _meta.id;
        }

        Meta& meta() {
            return _meta;
        }

        vector<uint8_t>& body() {
            return _body;
        }

        using Serial = SerializedObject<T>;

        friend std::ostream& operator << (std::ostream& os, const Serial& msg) {
            os << "ID: " << msg._meta.id << " ByteSize: " << msg._meta.bytes;
            return os;
        }

        friend Serial& operator << (Serial& msg, const string& data) {
            vector<uint8_t> v(data.begin(), data.end());
            msg << v;

            return msg;
        }

        template <typename DataType>
        std::enable_if_t<std::is_trivially_copyable<DataType>::value, Serial&>
        friend operator << (Serial& msg, const DataType& data) {
            size_t i = msg._body.size();

            msg._body.resize(i + sizeof(data));
            std::memcpy(msg._body.data() + i, &data, sizeof(DataType));

            return msg;
        }

        template <typename DataType>
        std::enable_if_t<std::is_trivially_copyable<DataType>::value, Serial&>
        friend operator << (Serial& msg, const vector<DataType>& data) {
            size_t vectorBytes = data.size() * sizeof(DataType);
            msg << vectorBytes;

            size_t i = msg._body.size();
            msg._body.resize(i + vectorBytes);
            std::memcpy(msg._body.data() + i, data.data(), vectorBytes);

            return msg;
        }

        template <typename DataType>
        std::enable_if_t<is_base_of_template<Serializable, DataType>::value, Serial&>
        friend operator << (Serial& msg, DataType& data) {
            msg << data.serializeObj().body();

            return msg;
        }

        template <typename DataType>
        std::enable_if_t<is_base_of_template<Serializable, DataType>::value, Serial&>
        friend operator << (Serial& msg, vector<DataType>& data) {
            msg << data.size();

            for (auto& e : data) {
                auto& s = e.serializeObj().body();
                msg << s;
            }

            return msg;
        }

        // ============================================================================================================
        // ============================================================================================================
        // ============================================================================================================
        // ============================================================================================================

        friend Serial& operator >> (Serial& msg, string& data) {
            vector<uint8_t> v{};
            msg >> v;

            data = string(v.begin(), v.end());;
            return msg;
        }

        template <typename DataType>
        std::enable_if_t<std::is_trivially_copyable<DataType>::value, Serial&>
        friend operator >> (Serial& msg, DataType& data) {
            size_t i = msg._desOffset;

            std::memcpy(&data, msg._body.data() + i, sizeof(DataType));
            msg._desOffset += sizeof(DataType);

            return msg;
        }

        template <typename DataType>
        std::enable_if_t<std::is_trivially_copyable<DataType>::value, Serial&>
        friend operator >> (Serial& msg, vector<DataType>& data) {
            size_t vectorBytes{0};
            msg >> vectorBytes;

            size_t i = msg._desOffset;
            data.resize(vectorBytes / sizeof(DataType));
            std::memcpy(data.data(), msg._body.data() + i, vectorBytes);
            msg._desOffset += vectorBytes;

            return msg;
        }

        template <typename DataType>
        std::enable_if_t<is_base_of_template<Serializable, DataType>::value, Serial&>
        friend operator >> (Serial& msg, DataType& data) {
            msg >> data._serial.body();
            data.deserializeObj();
            return msg;
        }

        template <typename DataType>
        std::enable_if_t<is_base_of_template<Serializable, DataType>::value, Serial&>
        friend operator >> (Serial& msg, vector<DataType>& data) {

            size_t size = 0;
            msg >> size;
            data.resize(size);

            for (auto& e : data) {
                msg >> e;
            }

            return msg;
        }

    private:
        Meta _meta;
        vector<uint8_t> _body;

        size_t _desOffset{0};
    };
}
