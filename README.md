# ВВЕДЕНИЕ
liblog - это библиотека для удобной записи сообщений в файл.
Сообщение состоит из текста, важности сообщения и времени записи сообщения(записывается локальное время)
# БЫСТРОЕ НАЧАЛО
Склонируйте проект себе в проект и запустите make
```bash
git clone https://github.com/YamatoCPP/Liblog.git
cd Liblog
make
```
Чтобы запустить приложение 
```bash
./build/main logFilePath defaultMessageImportance
# logFilePath - абсолютный или относительный путь до лог файла(txt формата)
# defaultMessageImportance - значение важности сообщения по умолчанию(low, medium, critical)
```
# ЗАПИСЬ СООБЩЕНИЙ
После запуска приложения вас встретит надпись предлагающая отправить пустое сообщение для конца ввода
Чтобы записать введите сообщение, а также его важность(оставьте поле пустым, если хотите поставить важность по умолчанию)
```bash
Please enter a blank message to end your entry.
Enter a message: Hello world!
Enter a message importance(low, medium, critical): low
```
Если ввести некорректную важность, собщение записано не будет.
```bash
Please enter a blank message to end your entry.
Enter a message: Hello world!
Enter a message importance(low, medium, critical): LoW
```
Запись сообщений будет продолжаться, пока вы не введете пустое сообщение.
