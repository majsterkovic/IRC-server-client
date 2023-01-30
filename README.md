# Komunikator internetowy typu IRC

* * *

## 1. Opis protokołu komunikacyjnego

Komunikacja między klientem a serwerem odbywa się przez protokół TCP i interfejs gniazd BSD. Dane są przekazywane w
postaci bajtów. Sa one dekodowane przez odpowiednie funkcje każdego z programów (klienta i serwera).

## 2. Opis implementacji

### 2.1 Serwer

Serwer został zaimplementowany w klasie `Server`, korzysta ona z klas `Client`, która zawiera pola:

```cpp
int descriptor;
char action{};
int room{};
std::string name;
```

Opisują one każdego klienta, który zostanie podłączony do serwera:
- deskryptor, na którym dołączył,
- akcję, którą chce wykonać (logowanie, wysłanie wiadomości, zmiana pokoju czy opuszczenie serwera),
- numer pokoju, w którym obecnie się znajduje,
- nazwę użytkownika.

Nowy użytkownik jest obsługiwany w wątku, który jest
tworzony po akceptacji każdego klienta. Wtedy odbywa się właściwa komunikacja, gdzie serwer odbiera od użytkownika znak
akcji, którą chce wykonać i wywołuje odpowiedną funkcję. Przesył wiadomości odbywa się poprzez funkcję, według
następującej konwencji:
`xxxxyyyy`

- xxxx – cztery bajty, na których zapisana jest wielkośc wiadomości
- yyyy – właściwa wiadomość.
  
Program wykorzystuje mutexy odpowiadające za synchronizację podczas współbieżnego wysyłania wiadomości do określonego
pokoju oraz operacji na wektorze klientów.

### 2.2 Klient

Projekt klienta składa się z klasy `IRC` opisującej działanie interfejsu graficznego oraz akcji niezbędnych do komunikacji z serwerem, klasy `Main`, która tworzy nowy
wątek służący do odbierania wiadomości oraz klasy `Connection` służącej do ustanowienia połączenia.

## 3. Opis sposobu kompilacji i uruchomienia serwera

### 3.1 Kompilacja i uruchomienie serwera

Po pobraniu całego projektu należy przejść do katalogu Server za pomocą polecenia `cd Server`. Aby skompilować pliki źródłowe z kodem serwera, należy wykonać polecenie:

```shell
make
```

Po udanej kompilacji można uruchomić serwer za pomocą polecenia:

```shell
./Server.out portnumber
```

gdzie portnumber można zastąpić numerem portu, na którym ma działać aplikacja (domyślnie 1234).

### 3.2 Kompilacja i uruchomienie klienta

Po pobraniu całego projektu należy przejść do katalogu Client za pomocą polecenia `cd Client`. Aby skompilować pliki źródłowe z kodem klienta, należy wykonać polecenie:

```powershell
mvn -B package --file pom.xml
```

Po udanej kompilacji można uruchomić klienta za pomocą polecenia:

```powershell
java -jar ./target/irc-client-swing-1.0-SNAPSHOT.jar address portnumber
```

gdzie zamiast `address` należy podać adres IP serwera, a jako `portnumber` można podać numer portu,
z którego ma korzystać aplikacja (domyślnie jest to 1234).
