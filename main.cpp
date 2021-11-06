#include <iostream>
#include <ctime>       
#include <string>

using namespace std;

int i;

//STRUCT DE VAGAS
struct vaga {

  struct tm data_hora_entrada;
  struct tm data_hora_saida;
  string placa;
  bool ocupada;

};

//FUNÇÃO ESCOLHER MODO CLIENTE OU ADMIN
int escolher_modo(){

  int modo;

  cout<<"\n----------------------------------\n";
  cout<<"Qual Modo deseja acessar?\n";
  cout<<"\n";
  cout<<"(1) - Cliente\n(2) - Administrador\n";
  cout<<"\n";
  cin>>modo;
  
  return modo;
}

//FUNÇÃO QUE CALCULA O PREÇO BASEADO NA DIFERENÇA DE TEMPO (SAÍDA E ENTRADA)
float calcula_total_a_pagar(struct tm data_inicial, struct tm data_final, float preco_por_hora){

  double segundos, horas;

  segundos = difftime(mktime(&data_final), mktime(&data_inicial));
  horas = segundos / 3600;

  return horas * preco_por_hora;
}

// FUNÇÃO PARA TRATAR DATA E HORA ADEQUADAMENTE
struct tm recebe_data_hora() {
  
  struct tm data_hora = {0};
  int dia, mes, ano, horas, minutos;
  int tipo;
  char barra;
  char hifen;
  char dois_pontos;

  bool formato_valido = false;

  cout<<"e o horario no formato dd/mm/aaaa-hh:mm (exemplo: 01/01/2020-16:30):" << endl;
  cout<<"\n";

  // VALIDAÇÃO DO FORMATO DE DATA E HORA
  while(formato_valido==false) {

    cin>> dia >> barra >> mes >> barra >> ano >> hifen >> horas >> dois_pontos >> minutos;

    if(barra =='/' || hifen == '-' || dois_pontos == ':'){
      formato_valido = true;
    }
    else {
      cout<<"\n";
      cout << "Formato inválido. Digite a data e a hora novamente: " << endl;
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout<<"\n";
    }
  } 
  data_hora.tm_mday = dia;
  data_hora.tm_mon = mes - 1;
  data_hora.tm_year = ano - 1900;
  data_hora.tm_hour = horas;
  data_hora.tm_min = minutos;

  return data_hora;
}

// FUNÇÃO RECEBE A PLACA E CHECA SE A MESMA JÁ EXISTE NA ENTRADA
string recebe_placa(struct vaga vagas_carro[60]) {

  string placa; 
  bool valido = false;
  cout<<"\n";
  cout<<"Por favor, insira a placa do seu veículo sem hífen: ";
  cin >> placa;
  cout<<"\n";
  
  while (valido==false) {
    int i = 0;
    for (; i< 60; i++){
      if (placa == vagas_carro[i].placa){
        cout<<"Veículo já guardado. Por favor, digite novamente a placa do seu veículo." << endl;
        cout<<"\n";
        break;
      }
    }
    if (i==60)
      valido = true;
    else
      cin >> placa;
  }
  return placa;
}

int main (){

  int tipo;
  string retirar_placa_carro, retirar_placa_camionete;
  float preco, valor_devido, dinheiro, troco;
  int modo, operacao;
  int contador_lotacao=0, contador_carro=0, contador_camionete=0;
  float fatur_carro=0, fatur_camionete=0, fatur_total=0;
  float fatur_medio_hora = 0;
  double horas, segundos, contador_tempo_medio;
 
  //60 VAGAS DE CARRO
  struct vaga vagas_carro[60];
  //60 VAGAS DE CAMIONETE
  struct vaga vagas_camionete[60];

  //INICIALIZANDO VALOR DOS ARRAYS DE CARRO E CAMIONETE
  for (int i=0; i < 60; i++) {
    vagas_carro[i].data_hora_entrada = {0};
    vagas_carro[i].data_hora_saida = {0};
    vagas_carro[i].placa = "";
    vagas_carro[i].ocupada = false;
  }

  for (int i=0; i < 60; i++) {
    vagas_camionete[i].data_hora_entrada = {0};
    vagas_camionete[i].data_hora_saida = {0};
    vagas_camionete[i].placa = "";
    vagas_camionete[i].ocupada = false;
  }

  modo = escolher_modo();

  while (modo==1|| modo==2){
    //OPERANDO MODO CLIENTE
    if (modo==1){

      cout<<"\n";
      cout<<"Seja bem-vindo ao nosso Estacionamento!\nDigite o número correspondente ao tipo de veículo: (1) - Carro | (2) - Camionete\n";
      cout<<"\n";
      cin>>tipo;

      //MANIPULANDO CARRO
      if (tipo==1){

        cout<<"\n";
        cout<<"Qual operação deseja realizar? (1) - Guardar veículo | (2) - Retirar veículo\n";
        cout<<"\n";
        cin>>operacao;

        if (operacao==1){

          int posicao_vaga = 0;
          //PROCURANDO A PRIMEIRA VAGA LIVRE DE CARRO
          for (; posicao_vaga < 60; posicao_vaga++) {
            if (vagas_carro[posicao_vaga].ocupada == false)
              break;
          }

          if (posicao_vaga==60) {
            contador_lotacao++;
            cout<<"Pedimos desculpas, estamos lotados!";cout<<"\n";
          }
          else {
            struct tm data_inicial; string placa;
            cout<<"\n";
            cout<<"Digite a hora inicial ";
            data_inicial = recebe_data_hora();
            placa = recebe_placa(vagas_carro);
            vagas_carro[posicao_vaga].placa = placa;
            vagas_carro[posicao_vaga].data_hora_entrada = data_inicial;
            vagas_carro[posicao_vaga].ocupada = true;
            contador_carro++;
            
          }
        }
        //OPERAÇÃO "RETIRADA DE CARRO"
        if (operacao==2){

          cout<<"Digite a placa do veículo: ";
          cout<<"\n";
          cin>>retirar_placa_carro;

          preco=5;
        
          int i = 0;
          for (; i<60; i++){
            if (retirar_placa_carro==vagas_carro[i].placa){
              
              cout<<"\n";
              cout<<"Digite a hora da saída ";
              
              vagas_carro[i].data_hora_saida = recebe_data_hora();

              valor_devido = calcula_total_a_pagar(vagas_carro[i].data_hora_entrada, vagas_carro[i].data_hora_saida, preco);

              cout<<"O total devido é: R$ "<<valor_devido<<endl;
              cout<<"\n";
              cout<<"Obrigado por utilizar nossos serviços!"<<endl;
              cout<<"\n";

              fatur_carro = valor_devido + fatur_carro;

              segundos = difftime(mktime(&vagas_carro[i].data_hora_saida), mktime(&vagas_carro[i].data_hora_entrada));
              horas = segundos / 3600;

              contador_tempo_medio = horas + contador_tempo_medio;

              vagas_carro[i].data_hora_entrada = {0};
              vagas_carro[i].data_hora_saida = {0};
              vagas_carro[i].placa = "";
              vagas_carro[i].ocupada = false;
              break;        
            }
          }
          if (i==60) {
            cout << "Placa não encontrada. Refaça a operação." << endl;
            cout<<"\n";
          }
        }
      }
      //MANIPULANDO CAMIONETE
      if (tipo==2){

        cout<<"Qual operação deseja realizar? (1) - Guardar veículo | (2) - Retirar veículo\n";
        cout<<"\n";
        cin>>operacao;

        if (operacao==1){

          int posicao_vaga = 0;
          //  PROCURANDO A PRIMEIRA VAGA LIVRE
          for (; posicao_vaga < 60; posicao_vaga++) {
            if (vagas_camionete[posicao_vaga].ocupada == false)
              break;
          }

          if (posicao_vaga==60) {
            contador_lotacao++;
            cout<<"Pedimos desculpas, estamos lotados!";
            cout<<"\n";
          }
          
          else {
            struct tm data_inicial; string placa;
            data_inicial = recebe_data_hora();
            placa = recebe_placa(vagas_camionete);
            vagas_camionete[posicao_vaga].placa = placa;
            vagas_camionete[posicao_vaga].data_hora_entrada = data_inicial;
            vagas_camionete[posicao_vaga].ocupada = true;
            contador_camionete++;
          }
        }
        //RETIRANDO CAMIONETE DO ESTACIONAMENTO
        if (operacao==2){

          cout<<"Digite a placa do veículo: ";
          cout<<"\n";
          cin>>retirar_placa_camionete;

          preco=7;

          int j = 0;
          for (; j<60; j++){
            if (retirar_placa_camionete==vagas_camionete[j].placa){
              
              cout<<"\n";
              cout<<"Digite a hora da saída ";
              
              vagas_camionete[j].data_hora_saida = recebe_data_hora();

              valor_devido = calcula_total_a_pagar(vagas_camionete[j].data_hora_entrada, vagas_camionete[j].data_hora_saida, preco);

              cout<<"O total devido é: R$ "<<valor_devido<<endl;
              cout<<"\n";
              cout<<"Obrigado por utilizar nossos serviços!"<<endl;
              cout<<"\n";

              fatur_camionete = valor_devido + fatur_camionete;

              segundos = difftime(mktime(&vagas_camionete[i].data_hora_saida), mktime(&vagas_camionete[i].data_hora_entrada));
              horas = segundos / 3600;

              contador_tempo_medio = horas + contador_tempo_medio;

              vagas_camionete[j].data_hora_entrada = {0};
              vagas_camionete[j].data_hora_saida = {0};
              vagas_camionete[j].placa = "";
              vagas_camionete[j].ocupada = false;
              break;
            }
          }
          if (j==60) {
            cout << "Placa não encontrada. Refaça a operação." << endl;
            cout<<"\n";
          }
        }
      }
    }
    //MODO ADMINISTRADOR
    if (modo==2){

      fatur_total = fatur_carro+fatur_camionete;

      fatur_medio_hora = (fatur_total/contador_tempo_medio)*(contador_carro+contador_camionete);

      cout<<"Relatório Atualizado de Movimento:\n";
      cout<<"\n";
      cout<<"Quantidade de carros atendidos: "<<contador_carro<<endl;
      cout<<"Faturamento total de carros: "<<fatur_carro<<endl;
      cout<<"Quantidade de camionetes atendidas: "<<contador_camionete<<endl;
      cout<<"Faturamento total de camionetes: "<<fatur_camionete<<endl;
      cout<<"Quantidade de veículos não atendidos por lotação: "<<contador_lotacao<<endl;
      cout<<"Faturamento médio por hora: "<<fatur_medio_hora<<endl;
      cout<<"Faturamento total: "<<fatur_total<<endl;

    }
    modo = escolher_modo();
  }
  return 0;
}