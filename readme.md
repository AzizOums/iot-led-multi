# Setup

## Si vous voulez faire un set up en local

- installer docker et docker-compose https://docs.docker.com/get-docker/
- executer `docker compose up -d` dans le dossier docker
- installer mosquitto et configurer un accès avec username et mot de passe ou utiliser `test.mosquitto.org`
- http://www.steves-internet-guide.com/install-mosquitto-linux/

## imports node-red et grafana

Dans node red
- Installer node-red-contrib-influxdb et node-red-dashboard (dans options -> manage palette)
- Ensuite importer les flows depuis le fichier `node-red-flows.json` (dans option -> importer)

Dans importez le dashboard depuis `grafana-dashboard.json`

## Si vous voulez utiliser la machine EC2

- Node-red est accessible via l'adresse http://35.180.55.122:1880/
- InfluxDB est accessible via l'adresse http://35.180.55.122:8086/ data base: iot, user: user, password: password
- Grafana est accessible via l'adresse http://35.180.55.122:3000/ avec les accès user: admin, password: ESP32Password
- Le serveur MQTT est accessible via l'adresse http://35.180.55.122:1884/ avec les accès user: ESP32, password: ESP32Password

A priori aucun changement à faire dans ce cas