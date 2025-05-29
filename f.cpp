#include <iostream>


class BoolVector {
private:
    char* data;
    size_t bytes;
    size_t bits;

public:
    // Конструктор по умолчанию
    BoolVector() {
        data = nullptr;
        bytes = 0;
        bits = 0;
    }

    // Деструктор
    ~BoolVector() {
        delete[] data;
    }

    // Установка конкретного бита в 0
    void set_0(size_t index) {
        size_t index_byte = index / 8;  // В каком байте находится нужный бит
        size_t index_bit = index % 8;   // Позиция бита в байте (0-7)
        data[index_byte] &= ~(1 << index_bit);
        // сдвигаем 1 влево на кол-во битов, меняем ее на 0 и умножением меняем в исходном векторе
    }

    // Установка конкретного бита в 1
    void set_1(size_t index) {
        size_t index_byte = index / 8;
        size_t index_bit = index % 8;
        data[index_byte] |= (1 << index_bit);
        // тоже, но устанавливаем 1
    }

    // Получение значения бита по индексу
    bool get(size_t index) {
        size_t index_byte = index / 8;
        size_t index_bit = index % 8;
        return (data[index_byte] >> index_bit) & 1;
    }

    // Оператор доступа по индексу
    bool operator[](size_t index) {
        return get(index);
    }

    // Универсальный метод установки бита
    void set_bit(size_t index, bool value) {
        if (value)
            set_1(index);
        else
            set_0(index);
    }

    // Добавление бита в конец вектора
    void push_back(bool value) {
        // Если память заполнена - увеличиваем
        if (bits == bytes * 8) {
            size_t new_bytes = bytes ? bytes * 2 : 1;  // Удваиваем или берём 1 байт
            char* newdata = new char[new_bytes];      // Выделяем новую память

            // Копируем старые данные
            for (size_t i = 0; i < bytes; i++) {
                newdata[i] = data[i];
            }

            delete[] data;    // Освобождаем старую память
            data = newdata;   // Обновляем указатель
            bytes = new_bytes; // Запоминаем новый размер
        }
        set_bit(bits, value);  // Устанавливаем бит
        bits++;                // Увеличиваем счётчик битов
    }

    // Возвращает текущее количество битов
    size_t size() {
        return bits;
    }

    // Вставка бита в произвольную позицию
    void insert(size_t index, bool value) {
        // Сдвигаем все биты после index вправо
        for (size_t i = bits - 1; i > index; i--) {
            set_bit(i, get(i - 1));
        }
        set_bit(index, value);  // Вставляем новый бит
        bits++;                // Увеличиваем счётчик
    }

    // Удаление бита из произвольной позиции
    void erase(size_t index) {
        // Сдвигаем все биты после index влево
        for (size_t i = index; i < bits - 1; i++) {
            set_bit(i, get(i + 1));
        }
        bits--;  // Уменьшаем счётчик
    }

    void print() {
        std::cout << "Bits (" << bits << "): [ ";
        for (size_t i = 0; i < bits; ++i) {
            // Получаем значение бита
            size_t byte_idx = i / 8;
            size_t bit_idx = i % 8;
            bool bit_value = (data[byte_idx] >> bit_idx) & 1;

            // Выводим бит (1 или 0)
            std::cout << bit_value << " ";

            // Добавляем разделитель каждые 8 бит для удобства чтения
            if ((i + 1) % 8 == 0 && (i + 1) != bits) {
                std::cout << "| ";
            }
        }
        std::cout << "]" << std::endl;
    }
};

int main() {
    std::setlocale(LC_ALL, "Russian");
    BoolVector a;

    a.push_back(true);
    for (int i = 0; i < 7; i++) a.push_back(0);
    a.print();

    std::cout << "Удаляем бит с индексом 1" << std::endl;
    a.erase(1);  // Удаляем второй бит (индекс 1)
    a.print();

    std::cout << "Ставим в начало 1" << std::endl;
    a.insert(0, 1);
    a.print();

    std::cout << "Ставим 0 в начало" << std::endl;
    a.set_0(0);
    a.print();

    a.push_back(1);
    a.print();

    for (int i = 0; i < 8; i++) a.push_back(1);
    a.print();
    return 0;
}
