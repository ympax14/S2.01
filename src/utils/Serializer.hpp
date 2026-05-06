#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <QMetaEnum>

namespace Serializer {
    template<typename T> int serializeEnumValue(T value) {
        return QMetaEnum::fromType<T>().valueToKey(value);
    }
}

#endif // SERIALIZER_HPP
