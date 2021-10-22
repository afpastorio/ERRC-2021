# Prática 2: Comunicação LoRaWAN com Servidor de Rede TTS

A comunicação com um servidor de rede TTS é possível utilizando o _gateway_ da Radioenge. O próprio fabricante disponibiliza um tutorial demonstrando como realizar a instalação do _firmware_ do _gateway_ e como registrá-lo na rede. Como a plataforma TTS sofreu mudanças somente o seu registro será abordado.

Acessando a [TTS](https://au1.cloud.thethings.network) é possível realizar o _login_, ou cadastro, na plataforma. Uma vez feita a autenticação o console é exibido apresentando as seções de aplicações e _gateways_. Para registrar um _gateway_ entramos na área dos _gateways_ e clicamos em _Add Gateway_. Neste momento três parâmetros devem ser configurados, _Gateway-ID_, _Gateway EUI_ e _Frequency Plan_. _Gateway-ID_ se refere a um identificador único que vai dar nome ao _gateway_. _Gateway EUI_ também é um identificador único do dispositivo, mas este é fornecido pelo fabricante. Já _Frequency Plan_ está relacionado ao plano de frequência a ser usado, no caso Australia 915-928 MHz, FSB2. O processo também pode ser visualizado no vídeo no [YouTube](https://youtu.be/vD2TaFjzCWI?t=117) a partir de 1:57.

<p align="center">
  <img width="600" src="img/gateway-console.svg">
</p>

Com o _gateway_ configurado é momento de adicionar uma aplicação. Na seção _Applications_ clique em _Add Application_. De forma bem simples preencha os campos como informado, aqui somente o _Application ID_ é obrigatório.

No painel da aplicação precisamos adicionar um dispositivo final. Basta clicar em _Add End Device_. Mudamos para configuração manual e selecionamos a versão LoRaWAN e o plano de frequência. As chaves podem ser geradas automaticamente, destaque para AppEUI que deve ter pelo menos um número diferente de zero. Esse processo pode ser visto na figura abaixo. 

<p align="center">
  <img width="600" src="img/end-registration.svg">
</p>

Para a programação do dispositivo será utilizada a última versão da biblioteca [arduino-lmic](https://github.com/mcci-catena/arduino-lmic). Podendo ser instalada tanto pelo gerenciador de bibliotecas quanto manualmente. O código está disponibilizado neste repositório, sendo necessário somente modificar as chaves para corresponder com o dispositivo criado conforme apresentado abaixo.

```cpp
  // AppEUI em formato LSB
  static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
  // DevEUI em formato LSB
  static const u1_t PROGMEM DEVEUI[8] = {0x9B, 0x40, 0x04, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
  // AppKey em formato MSB
  static const u1_t PROGMEM APPKEY[16] = {0xA9, 0xE2, 0x67, 0x57, 0x97, 0xBF, 0x68, 0x34, 0xE1, 0x2D, 0xCA, 0x0D, 0x9F, 0x48, 0x44, 0xCD};
  }
```

Outro ponto importante a ser considerado é a variável que armazena a mensagem e a função que faz o envio da mensgem. Estamos enviando uma mensagem contendo a frase _"Hello World!"_ atribuída à variável _txBuffer_. A rotina responsável pelo envio da mensagem é _do\_send_. Ela é configurada para um envio periódico, que pode ser alterado através da variável _TX\_INTERVAL_.

Uma vez que o dispositivo foi programado basta ligá-lo e o tráfego de mensagens começará a aparecer na console TTS. Inicialmente os dados serão mostrados em forma de _bytes_, portanto faz-se necessário a decodificação da menagem. Acessando a aba _Payload Formatters - Uplink_ podemos incluir um código para formatar a mensagem. O código pode ser visualizado no algoritmo abaixo, onde aplicamos a transformação para _String_ somente aos _bytes_ do _payload_ completo, _input_.

~~~javascript
  function decodeUplink(input) { 
    return {
        data: {
            msg: String.fromCharCode.apply(null, input.bytes)
        },
        warnings: [],
        errors: []
    };
}
~~~

<p align="center">
  <img width="600" src="img/live-data.svg">
</p>

Exemplificando como enviar as mensagens para um servidor de aplicação utilizaremos a plataforma [TagoIO](https://tago.io/). Para isso, acessamos o site e geramos uma nova chave de autorização, em _Devices_ clicando em _Authorization_. Nesta seção pode-se dar um nome a chave e clicar em _Generate_. Está chave é necessária para realizar a integração entre TTS e TagoIO. Ainda em _Devices_, adicionamos um novo dispositivo clicando em _Add Device_. Selecionamos a opção de dispositivo customizado presente em _LoRaWAN TTI/TTN v3_, atribuímos um nome ao dispositivo e colocamos o seu respectivo _DevEUI_.

<p align="center">
  <img width="600" src="img/tago-devices.svg">
</p>

De volta a TTS, adiconamos um _WebHook_ para à TagoIO em _Integrations_, selecionando a opção TagoIO, como demonstrado na Figura abaixo. Neste momento, inserimos um nome para a integração e a chave gerada anteriormente pela TagoIO. Finalizando o processo de criação da integração ao clicar em _Create tagoio webhook_.

<p align="center">
  <img width="600" src="img/tago-tts.svg">
</p>

Alterando o código para enviar um valor de um contador conforme pressionamos um botão e também _Payload Formatter_ para decodificar os _bytes_ em um número inteiro, como no código abaixo.

~~~javascript
function decodeUplink(input){
    return { 
        data: {
            counter: (input.bytes[0] << 8) +  input.bytes[1]
        },
        warnings: [],
        errors: []
    };
}
~~~

Nesta etapa a plataforma TagoIO fornece a criação de painéis. Estes painéis são formados por diferentes _widgets_, que acabam por exibir as informações recebidas do dispositivo. Para criar um painel clicamos em \textit{Add Dashboard} e em _Add Widget_  em seguida. Após clicar em _Add Widget_ as opções de _widget_ serão mostradas.

<p align="center">
  <img width="600" src="img/tago-dash.svg">
</p>

Como a variável a ser exibida é do tipo numérica, adicionamos um _Display_. Ao selecionar o _widget_ a ser adicionado um outro painel é aberto para configurá-lo. Aqui selecionamos o dispositivo criado e a variável a ser exibida, no caso, _counter_ que foi definida no _Payload Formatter_.

<p align="center">
  <img width="600" src="img/tago-widget.svg">
</p>

<p align="center">
  <img width="600" src="img/tago-display.svg">
</p>


Desta forma, toda vez que o botão do ESP32 Heltec for pressionado, o valor de _counter_ será incrementado e enviado para o servidor de rede TTS. O servidor decodifica e encaminha a mensagem para o servidor de aplicação, TagoIO, que por sua vez faz a exibição dos dados. E pode ser visto no painel público disponibilizado, [TagoERRC](https://admin.tago.io/public/dashboard/6171707233935b001165d9f6/9a58c3d5-cf9c-4a15-880e-45e97ce1a1e5).
