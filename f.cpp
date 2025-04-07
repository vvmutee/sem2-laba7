#include <iostream>
#include <vector>
#include <stdexcept>

class BoolVector {
private:
    std::vector<uint8_t> data;
    size_t bit_size = 0;

    // Получить байт и маску по индексу
    static std::pair<size_t, uint8_t> getByteIndexAndMask(size_t index) {
        size_t byte_index = index / 8;
        uint8_t mask = 1 << (index % 8);
        return {byte_index, mask};
    }

public:
    // Прокси-класс для доступа к отдельным битам
    class Reference {
        friend class BoolVector;
        uint8_t& byte;
        uint8_t mask;

        Reference(uint8_t& b, uint8_t m) : byte(b), mask(m) {}

    public:
        operator bool() const {
            return byte & mask;
        }

        Reference& operator=(bool value) {
            if (value)
                byte |= mask;
            else
                byte &= ~mask;
            return *this;
        }

        Reference& operator=(const Reference& other) {
            return *this = static_cast<bool>(other);
        }
    };

    // Добавление в конец
    void push_back(bool value) {
        if (bit_size % 8 == 0) {
            data.push_back(0);
        }
        auto [byte_index, mask] = getByteIndexAndMask(bit_size);
        if (value)
            data[byte_index] |= mask;
        ++bit_size;
    }

    // Метод size
    size_t size() const {
        return bit_size;
    }

    // Получение значения по индексу
    bool operator[](size_t index) const {
        if (index >= bit_size) throw std::out_of_range("Index out of bounds");
        auto [byte_index, mask] = getByteIndexAndMask(index);
        return data[byte_index] & mask;
    }

    // Изменение значения по индексу
    Reference operator[](size_t index) {
        if (index >= bit_size) throw std::out_of_range("Index out of bounds");
        auto [byte_index, mask] = getByteIndexAndMask(index);
        return Reference(data[byte_index], mask);
    }

    // Метод insert
    void insert(size_t index, bool value) {
        if (index > bit_size) throw std::out_of_range("Index out of bounds");

        push_back(false); // расширить размер
        for (size_t i = bit_size - 1; i > index; --i) {
            (*this)[i] = (*this)[i - 1];
        }
        (*this)[index] = value;
    }

    // Метод erase
    void erase(size_t index) {
        if (index >= bit_size) throw std::out_of_range("Index out of bounds");

        for (size_t i = index; i < bit_size - 1; ++i) {
            (*this)[i] = (*this)[i + 1];
        }
        --bit_size;
        if (bit_size % 8 == 0)
            data.pop_back();
    }

    // Вывод содержимого вектора
    void print() const {
        for (size_t i = 0; i < bit_size; ++i)
            std::cout << (*this)[i] << " ";
        std::cout << '\n';
    }
};
int main() {
    BoolVector bv;
    bv.push_back(true);
    bv.push_back(false);
    bv.push_back(true);
    bv.push_back(true);
    std::cout << "Изначальный вектор: ";
    bv.print(); // 1 0 1 1

    std::cout << "Размер: " << bv.size() << "\n";

    std::cout << "bv[2] = " << bv[2] << "\n";
    bv[1] = true;
    std::cout << "После bv[1] = true: ";
    bv.print(); // 1 1 1 1

    bv.insert(2, false);
    std::cout << "После insert(false, 2): ";
    bv.print(); // 1 1 0 1 1

    bv.erase(3);
    std::cout << "После erase(3): ";
    bv.print(); // 1 1 0 1
}

