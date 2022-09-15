```sh
sudo apt install mosquitto
sudo apt install mosquitto-client
sudo apt install libmosquitto-dev

sudo service mosquitto start
sudo service mosquitto status

sudo journalctl -f -u mosquitto

mosquitto_pub.exe -p 1883 -t "topic/test" -m "123"
mosquitto_sub.exe -p 1883 -t "topic/test"
```
