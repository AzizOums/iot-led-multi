#ifndef _INIT_H_
#define _INIT_H_

#include <WiFi.h>
#include <WebServer.h>

void setupWifi();
void stopServer();

WebServer server(80);

#define SSID "initESP32"
#define PWD "0123456789"

IPAddress IP(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

const char homePage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<title>Init WiFi</title>

<body>
  <div class="container">
    <div class="inner">
      <div class="panel panel-left">
        <div class="panel-content">
          <div class="image-background">
          </div>
        </div>
      </div>

      <div class="panel panel-right">
        <div class="panel-content">

          <div class="form">
            <h1>Init MQTT & WIFI</h1>

            <div class="group">
              <input type="text" oninput="update('ssid', this.value)" required>
              <span class="highlight"></span>
              <label>Wifi SSID</label>
            </div>

            <div class="group">
              <input type="text" oninput="update('pwd', this.value)" required>
              <span class="highlight"></span>
              <label>WIFI password</label>
            </div>


            <button type="submit" class="send-btn" onclick="send()">
              Send
            </button>
          </div>

        </div>
      </div>
    </div>
  </div>
</body>

<script type="text/javascript">
  let payload = {
    ssid: "",
    pwd: "",
  };

  const update = (key, value) => {
    payload[key] = value;
  }

  const isReady = () =>
    !!payload.ssid && !!payload.pwd;

  const send = () => {
    if (!isReady()) {
      console.error(payload, "not ready");
      return;
    }
    const xhttp = new XMLHttpRequest();
    const params = Object.entries(payload).map(([k, v]) => `${k}=${v}`).join("&");
    xhttp.open("POST", `/init?${params}`, true);
    xhttp.send();
  }  
</script>

<style>
  html {
    width: 100%;
    height: 100%;
  }

  body {
    width: 100%;
    height: 100%;
    margin: 0;
    padding: 0;
    font-family: 'Open Sans', sans-serif;
    font-weight: 300;
    background: #FDFDFD;
  }

  .container {
    width: 100%;
    height: 100%;
    margin: 0;
    padding: 0;
    transition: all 0.3s ease;
  }

  .container .inner {
    width: 100%;
    height: 100%;
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    overflow: hidden;
  }


  .container .inner .panel {
    position: relative;
    box-sizing: border-box;
    height: 100%;
    width: 60%;
    float: left;
    transform: skew(-20deg);
  }

  .container .inner .panel.panel-left {
    margin-left: -10%;
  }

  .container .inner .panel .panel-content {
    position: absolute;
    width: 100%;
    height: 100%;
    left: 50%;
    top: 50%;
    transform: translateX(-50%) translateY(-50%) skew(20deg);
  }

  .container .inner .panel.panel-left .panel-content .image-background {
    position: absolute;
    width: 100%;
    height: 100%;
    left: 50%;
    top: 50%;
    opacity: 0.95;
  }

  .container .inner .panel.panel-right {
    margin-right: -10%;
    background: #1B1B1B;
  }

  .container .inner .panel.panel-right .panel-content .form {
    display: block;
    position: absolute;
    margin: 0 auto;
    width: 300px;
    min-height: 400px;
    top: 50%;
    left: 50%;
    transform: translateX(-50%) translateY(-25%);
  }

  .container .inner .panel.panel-right .panel-content .form h1 {
    display: block;
    margin: 0 0 45px 0;
    color: #B1B1B1;
    font-weight: 200;
    font-size: 18px;
  }

  .group {
    position: relative;
    margin-bottom: 25px;
  }

  input {
    font-size: 13px;
    height: 25px;
    padding: 10px 10px 10px 5px;
    display: block;
    width: 300px;
    border: none;
    outline: none;
    border-bottom: 1px solid #B1B1B1;
    color: #B1B1B1;
    background: rgba(0, 0, 0, 0);
    opacity: 0.5;
    transition: 0.2s ease;
  }

  input:focus {
    outline: none;
    opacity: 1;
  }

  label {
    color: #B1B1B1;
    font-size: 13px;
    font-weight: normal;
    position: absolute;
    pointer-events: none;
    left: 5px;
    top: 15px;
    opacity: 0.5;
    transition: 0.2s ease all;
    -moz-transition: 0.2s ease all;
    -webkit-transition: 0.2s ease all;
  }

  /* active state */
  input:focus~label,
  input:valid~label {
    top: -10px;
    font-size: 12px;
    color: #B1B1B1;
    opacity: 1;
  }

  .highlight {
    position: absolute;
    height: 60%;
    width: 100px;
    top: 25%;
    left: 0;
    pointer-events: none;
    opacity: 0.5;
  }

  /* active state */
  input:focus~.highlight {
    -webkit-animation: inputHighlighter 0.3s ease;
    -moz-animation: inputHighlighter 0.3s ease;
    animation: inputHighlighter 0.3s ease;
  }

  /* ANIMATIONS ================ */
  @-webkit-keyframes inputHighlighter {
    from {
      background: #B1B1B1;
    }

    to {
      width: 0;
      background: transparent;
    }
  }

  @-moz-keyframes inputHighlighter {
    from {
      background: #B1B1B1;
    }

    to {
      width: 0;
      background: transparent;
    }
  }

  @keyframes inputHighlighter {
    from {
      background: #B1B1B1;
    }

    to {
      width: 0;
      background: transparent;
    }
  }

  button.send-btn {
    all: unset;
    float: right;
    color: #B1B1B1;
    transition: 0.2s ease;
  }

  button.send-btn:hover {
    color: #FAFAFA;
    cursor: pointer;
  }
</style>

</html>
)=====";

#endif