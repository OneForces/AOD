#include <iostream>
#include <string>
#include <list>
#include <algorithm>
// Класс "Идентификатор"
class Identifier
{
public:
// Задаётся имя переменной при создании
    Identifier(const std::string &name):
            m_name(name)
    {
    }
public:
// Получается имя переменной
    std::string name() const
    {
        return m_name;
    }
private:
    std::string m_name;
};
// Необходимо для поиска переменной по имени
bool operator==(const Identifier &left, const Identifier &right)
{
    return left.name() == right.name();
}
// Функция, вычисляющая хэш
// Принимает идентификатор, хэш которого надо посчитать
// Возвращает вычисленный хэш
size_t hash(const Identifier &id)
{
// Если имя переменной составляет один символ - возвращается его код,
// умноженный на два
    if (id.name().length() == 1)
        return 2 * size_t(id.name()[0]);
// Иначе возвращается сумма кодов первых двух символов
    return size_t(id.name()[0]) + size_t(id.name()[1]);
}
// Класс исключение "Идентификатор не найден"
// Нужен для выдачи сообщения о том, что идентификатор не найден в таблице,

// наружу функции поиска идентификатора
class IDNotFoundException : std::exception
{
public:
    IDNotFoundException(const std::string id_name):
            m_what(std::string("Identifier \'") + id_name + "\' not found!")
    {
    }
    virtual ~IDNotFoundException() throw()
    {
    }
public:
    const char *what() const throw()
    {
        return m_what.c_str();
    }
private:
    std::string m_what;
};
// Класс "Хэш-таблица", основанная на методе цепочек
// Метод цепочек заключается в следующем: таблица представляет собой массив
// связных списков фиксированного размера. Вычисленный хэш-функцией хэш является
// индексом в этом массиве списков. Известно, что список по этому индексу будет
// содержать все идентификаторы, для которых функция вернула одинаковый хэш.
// Осталось только найти идентификатор в данном списке и возвратить ссылку на
// него.
class HashTable
{
public:
// Минимально возможное значение хэша
    static const size_t min_hash_value = int('A') + int('0');
// Максимально возможное значение хэша
    static const size_t max_hash_value = int('z') + int('z');
// Размер массива списков - хэш-таблицы
    static const size_t hash_table_size = max_hash_value - min_hash_value;
public:
// Добавление идентификатора в хэш-таблицу
    void add(const Identifier &id)
    {
// Добавление идентификатора в список, расположенный в таблице по
// индексу, вычисленному хэш-функцией (с учётом смещения)
        m_hash_table[hash(id) - min_hash_value].push_back(id);
    }

// Поиск идентификатора в таблице по имени
    Identifier &get(const std::string &id_name)
    {
// Сохраняется ссылка на список, в котором потенциально будет
// расположен идентификатор (для простоты)
        std::list<Identifier>& line = m_hash_table[hash(id_name) - min_hash_value];
// Поиск идентификаторы в списке по имени
        std::list<Identifier>::iterator it =
                std::find(line.begin(), line.end(), id_name);
// Если при поиске были просмотренны все элементы списка,и ни один не
// подошёл - сообщаем о том, что идентификатор не найден, посредством
// исключения
        if (it == line.end())
            throw IDNotFoundException(id_name);
// Если идентификатор найден - возвращаем ссылку на него
        return *it;
    }
private:
// Хэш-таблица - массив связных списков идентификаторов
    std::list<Identifier> m_hash_table[hash_table_size];
};
int main()
{
// Создаём хэш-таблицу
    HashTable ht;
// Добавляем в неё различные идентификаторы
    ht.add(Identifier("a"));
    ht.add(Identifier("aa"));
    ht.add(Identifier("if"));
    ht.add(Identifier("fi"));
// На случай, если идентификатор не будет найден, заворачиваем код поиска
// идентификаторов в блок try/catch
    try
    {
// Выводим на экран информацию о различных идентификаторах
        std::cout << ht.get("a").name() << std::endl;
        std::cout << ht.get("aa").name() << std::endl;
        std::cout << ht.get("if").name() << std::endl;
        std::cout << ht.get("fi").name() << std::endl;
// Проверяем случай, когда идентификатор не должен быть найден
        std::cout << ht.get("hello").name() << std::endl;
    }

    catch (const IDNotFoundException &ex)
    {
// Если идентификатор не найден - сообщаем об этом
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}