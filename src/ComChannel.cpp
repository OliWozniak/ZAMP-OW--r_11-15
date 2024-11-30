#include "ComChannel.hh"

ComChannel::ComChannel(/* args */){}

ComChannel::~ComChannel(){
}

// void ComChannel::Init(int Socket){
//   _Socket4Sending = Socket;
// }

// int ComChannel::GetSocket() const{
//   return _Socket4Sending;
// }

// std::mutex &ComChannel::UseGuard(){return _Access;}

/*!
 * Otwiera połączenie sieciowe
 * \param[out]  rSocket - deskryptor gniazda, poprzez które może być
 *                        realizowana komunikacja sieciowa.
 */
bool ComChannel::open_connection()
{
  struct sockaddr_in  DaneAdSerw;

  bzero((char *)&DaneAdSerw,sizeof(DaneAdSerw));

  DaneAdSerw.sin_family = AF_INET;
  DaneAdSerw.sin_addr.s_addr = inet_addr("127.0.0.1");
  DaneAdSerw.sin_port = htons(PORT);


  _Socket4Sending = socket(AF_INET,SOCK_STREAM,0);

  if (_Socket4Sending < 0) {
     std::cerr << "*** Blad otwarcia gniazda." << std::endl;
     return false;
  }

  if (connect(_Socket4Sending,(struct sockaddr*)&DaneAdSerw,sizeof(DaneAdSerw)) < 0)
   {
     std::cerr << "*** Brak mozliwosci polaczenia do portu: " << PORT << std::endl;
     return false;
   }
  (*this).send("Clear \n");
  return true;
}


/*!
 * \brief Wysyła napis do poprzez gniazdo sieciowe.
 *
 * Wysyła napis do poprzez gniazdo sieciowe.
 * \param[in] Sk2Server - deskryptor gniazda sieciowego, poprzez które 
 *                        ma zostać wysłany napis w sensie języka C.
 * \param[in] sMesg - zawiera napis, który ma zostać wysłany poprzez
 *                    gniazdo sieciowe.
 */
int ComChannel::send(const char *sMesg)
{
  std::cout << sMesg<< std::endl;
  ssize_t  IlWyslanych;
  ssize_t  IlDoWyslania = (ssize_t) strlen(sMesg);

  while ((IlWyslanych = write(_Socket4Sending,sMesg,IlDoWyslania)) > 0) {
    IlDoWyslania -= IlWyslanych;
    sMesg += IlWyslanych;
  }
  if (IlWyslanych < 0) {
    std::cerr << "*** Blad przeslania napisu. " << IlWyslanych << std::endl;
  }
  return 0;
}    

void ComChannel::LockAccess(){
  _Access.lock();
}

void ComChannel::UnlockAccess(){
  _Access.unlock();
}