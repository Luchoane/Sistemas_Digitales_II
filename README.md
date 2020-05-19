# Yara API - MercadoLibre IRT challenge - Luchoane

Para la instalación de docker, ejecutar `install.ps1` o ingresar los siguiente comandos:

```
docker build --tag=yara-challenge_irt-luchoane .
docker run -it -p 0.0.0.0:1234:1234 yara-challenge_irt-luchoane
```

Se instalaran las siguiente librería de python3 en docker:

```
yara-python
pandas
Flask
requests
```


## Endpoints: (dentro de la carpeta "testing" se pueden econtrar los .json de ejemplo para probar)


### Index
Metodo: `GET`
Path: `/`


`curl --url localhost:1234/`


Mensaje de bienvenida de la API. Cod: 200. Respuesta:

```
API Yara - Challenge Mercado Libre IRT - Lucho
```


### Add Rule
Metodo: `POST`
Path: `/api/rule`


`curl -X POST --url localhost:1234/api/rule -H "Content-type: application/json" -d @json_add.txt`


- json_add.txt


```
{
"name":"esto no es coca papi rule",
"rule":"rule EstoNoEsCocaPapiRule\r\n{\r\n strings:\r\n $my_text_string = \"prueba\"\r\n condition:\r\n $my_text_string\r\n}"
}

```


Suma una nueva regla a `reglas.yar`. En `reglas.csv` se agrega un nuevo número de ID y el nombre de la regla. Devuelve el nro de ID, nombre de la regla y regla yara. Cod: 201. Respuesta de ejemplo:


```
{"id": 1, "name": "EstoNoEsCocaPapiRule", "rule": "rule EstoNoEsCocaPapiRule\r\n{\r\n strings:\r\n $my_text_string = \"prueba\"\r\n condition:\r\n $my_text_string\r\n}"}
```


### Get Rules
Metodo: `GET`
Path: `/api/get_rules`


`curl --url localhost:1234/api/get_rules`


Parsea `reglas.yar` y muestra las reglas. Cod: 200. Respuesta de ejemplo:


```
rule EstoNoEsCocaPapiRule

{

 strings:

 $my_text_string = "prueba"

 condition:

 $my_text_string

}rule EstoNoEsCocaPapiRule2

{

 strings:

 $my_text_string = "prueba2"

 condition:

 $my_text_string

}
```


### Get IDS
Metodo: `GET`
Path: `/api/get_ids`


`curl --url localhost:1234/api/get_ids`


Parsea `reglas.csv` y muestra los ID de las reglas y sus nombres. Cod: 20. Respuesta de ejemplo:


```
id,nombre
1.0,EstoNoEsCocaPapiRule
2.0,EstoNoEsCocaPapiRule2
```


### Get Log
Metodo: `GET`
Path: `/api/get_log`


`curl --url localhost:1234/api/get_log`


Parsea `log.log` y muestra el log. Cod: 200. Respuesta de ejemplo:


```
19-05-2020 18:01 [req] {'name': 'esto no es coca papi rule', 'rule': 'rule EstoNoEsCocaPapiRule\r\n{\r\n strings:\r\n $my_text_string = "prueba"\r\n condition:\r\n $my_text_string\r\n}'}
19-05-2020 18:01 [res] ('{"id": 1, "name": "EstoNoEsCocaPapiRule", "rule": "rule EstoNoEsCocaPapiRule\\r\\n{\\r\\n strings:\\r\\n $my_text_string = \\"prueba\\"\\r\\n condition:\\r\\n $my_text_string\\r\\n}"}\n', 201)
```


### Analyze Text
Metodo: `POST`
Path: `/api/analyze/text`


`curl -X POST --url localhost:1234/api/analyze/text -H "Content-type: application/json" -d @json_text.txt`


- json_text.txt


```
{
"text": "estoesuntextoaprueba2analizar",
"rules": [
	{
		"rule_id": 1
	},
	{
		"rule_id": 2
	}
	]
}

```


Toma el texto para entregarle al driver de Yara y matchear con las reglas especificadas. Respuesta de ejemplo:


`{"status": "ok", "results": [{"rule_id": 1, "matched": true}, {"rule_id": 2, "matched": true}]}`


### Analyze File
Metodo: `POST`
Path: `/api/analyze/file`


`curl -X POST http://localhost:1234/api/analyze/file -H "Content-type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW" -F file="@prueba.txt" -F "rules=1,2"`


- prueba.txt


```
prueba
```


El driver de Yara analiza el file y matchea con las reglas especificadas. Cod: 200. Respuesta de ejemplo:


`{"status": "ok", "results": [{"rule_id": 1, "matched": true}, {"rule_id": 2, "matched": true}]}`


### Analyze URL
Metodo: `POST`
Path: `/api/analyze/url`


`curl -X POST --url localhost:1234/api/analyze/url -H "Content-type: application/json" -d @json_url.txt`


- json_url.txt


```
{
"url": "https://es.wikipedia.org/wiki/Lorem_ipsum",
"rules": [
	{
		"rule_id": 1
	},
	{
		"rule_id": 2
	}
	]
}
```


`requests` toma el contenido de la URL para que el driver de Yara analice el texto de la página y matchea con las reglas especificadas. Cod: 200. Respuesta de ejemplo:


`{"status": "ok", "results": [{"rule_id": 1, "matched": true}, {"rule_id": 2, "matched": false}]}`






























