

#include <pgmspace.h>

#define SECRET

const char ssid[] = "OnlineTPS";
const char pass[] = "TPS@12345";


///////////////////////////

const char comma[] = ",";


/////////////////////////

#define THINGNAME "Test123"

int8_t TIME_ZONE = -5; //NYC(USA): -5 UTC
//#define USE_SUMMER_TIME_DST  //uncomment to use DST

const char MQTT_HOST[] = "a2rc0pzumuju4u-ats.iot.us-east-1.amazonaws.com";

// Obtain First CA certificate for Amazon AWS
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// Copy contents from CA certificate here ▼
static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUOtLnGIbYqtuganlowlZ2BsIHajUwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDQyODA2NTgx
NVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANNfeijEO4GP0CQjpzgR
qwkApMGx0E5l0+We6AtsqZoXBAAa9ebRhm9Nsv9ojWRXgvXQmEvW2Izk4GkVzlo5
jEVuw42g9/iIU5J5L+OjTXfN1MciVfNwBeo/Lr+IA86779lZ7j0oH7SUEwmB/Zq/
XStGHXt1lBTplXztZzmGXOYdMhvTSM2jEcyc2D8WH6nCmeiYjKxqJ+ga1Qw9YMaO
AWTMkX5mBRsctWVSgd28nwxfAy9IIzyNbgtMN1CSOpTIBWyj2nQYqxUZsN8oQOWb
hIa1Y+i+kFC9olr+GENDqQgGzOyPud2xRqMEVhMCaTznmZgPmoSmZpooPadnFOdi
8o0CAwEAAaNgMF4wHwYDVR0jBBgwFoAUcin8vPpGTLNLCLxMIcsWXbAPxb0wHQYD
VR0OBBYEFFYFZOjgyYiuwG1x/4WaHTiXXVmOMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAzUJeL/lanwW+BlHLloG4SYdYY
FxA5MbUdKQWM15Bj9sjCSVyGMS3whZFyXU+t+9ek06Xy+iGEZXVDk5DXJKm9IyVd
gUBeqKazx33HWfVVjkkUpiZjKeHkwOID27vaz3G1i8fqRtlUFk/yMSKru9wFf2b9
NPI+XmwPIZIqDpgCBleavMueGpjEVTwRR4P01C0Hg+R0ZQnwEnocqXq8DwZbn9nb
z/pLCifAqxTm50y1I0slIj4+1zRLwCh1OoxxP+OUcNoDEEGczrBXVtvEW2I1uxQH
CqgIwl3OdRXeBSQvI5108DsUqwsc5ct/jdif07YUbUU4QR6+eoxwEUAT5N+V
-----END CERTIFICATE-----
)KEY";

// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA0196KMQ7gY/QJCOnOBGrCQCkwbHQTmXT5Z7oC2ypmhcEABr1
5tGGb02y/2iNZFeC9dCYS9bYjOTgaRXOWjmMRW7DjaD3+IhTknkv46NNd83UxyJV
83AF6j8uv4gDzrvv2VnuPSgftJQTCYH9mr9dK0Yde3WUFOmVfO1nOYZc5h0yG9NI
zaMRzJzYPxYfqcKZ6JiMrGon6BrVDD1gxo4BZMyRfmYFGxy1ZVKB3byfDF8DL0gj
PI1uC0w3UJI6lMgFbKPadBirFRmw3yhA5ZuEhrVj6L6QUL2iWv4YQ0OpCAbM7I+5
3bFGowRWEwJpPOeZmA+ahKZmmig9p2cU52LyjQIDAQABAoIBAQDDfELbXxj1q07B
jaPCbDy1V74IUW4og25kqAFNR5LXqr+NZWHUtiTR3ZdVNZnGmlTJCMFBKpMeQzL0
nTE6Koz7TgRXgO61EUfRaHkM2+PykA9gl/XL7efai9KtWprC4PdjGhQGxYwFEwlE
Os+UBbWiJvdvTf/FE36oilt0TTqvqdRk1/zwC9IV+7I1J857JHB5OmOXVInsI+KY
v1/yKCfuA8lsf6a5hlev0lGa0uzqqHrKmq3Lque8CxeLR0GtvmGh3EhnPPgUfvgR
m9wV+B7c5Za+yVgPHvEd4UuheNZ6BB+hWBBF+Vx9CGQiZiKYmyF+PuVF91YkEAVN
jmROeCK1AoGBAO2zVQwI/ao4UCAE14wW9flaQiBZOeuj7Zy6FmbTUutRBITQ0bFH
SD+qIBIbN4mkf8OAtQSNszrqk2Z6StKUSpCcEKk9w2N3DGKrTOVV5XKHnpnka1eD
jiQifYydAwpqeWzA+kgAGG2Hs8MMhybOSofzhYJ2C/bRinbtkC+ezv/nAoGBAOOl
Ri1+ybGWRvus+uCEF8fB1T5PNY1LVigjLUeD648ERxDP0fzXLL3prruT/WVCi8MX
mR63ILQY7QFH4ilDg9dEY1H5EleHuKcPxkQZhRBdtJhJNmikEXdQ12Ci/nb1zHyt
ji6E7nBByvISNZ2b9s76q7mJU9RQPLpQfGEJhntrAoGBAITd3icpC2nPqnILYtTq
rLmiCx574ZVm/1XA6u61Jz9j/QukkeHvP2Je3Shh0Ex/B++nDQgrkMjZAGWi7BoD
TB7DLcj8PjBmBiVNLWFv/x5i0nOhK6MyZQyrvtQPm/iBhbgVspD2hLEMzp2zbgRy
AVU2+STwiUWA4U4TAqqrsXd3AoGAVr6q4Dk5CCe4grymmJkdKqxM4sJyKJY1jXPi
FCdLwqKhpeFbJhqkXPXH9Ihu/X//G+dJHLAxkQ8SePN4Vbr5aTURF7Jub9Yy4c4c
AeUrlFHwuDrltbgXQ70VZSiXCKOfOT81N7rF1RbtC5MuxREiWjqg50FSJUwx59k1
nBByI2MCgYEAiocQwLcbe+j6rgXqEFPDcwnpVQbr0G2jwz3BUZHQlrE1KMdyPXAI
ZDgQ7ht1syyYVRP8O3VDJ9VPbKwbnXz0lxy38fVy8MXdPJ6uQxM3z7XrGotsLPPl
7IHOmJRwpakwTDY7b2ma+RN5R1b37L5QittT4ix4ggrxf0ThUlekK70=
-----END RSA PRIVATE KEY-----
)KEY";
