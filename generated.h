//RGB Rotate -> rr
//Random -> r
//Dot -> d
//HSV Pulse -> hp
//Solid -> s
//Pulse -> p
//Pulse Random -> pr
//Fireworks -> f
//Waves -> w
//Shift -> sh
//Brians Function -> bf
//Brians Function 2 -> bf2
//Brians Function Static -> bfs
//Off -> o
struct Firework {int location; byte maxSpread; int lifetime; byte hue;};
struct MovingVertex {float location; int iloc; int16_t finalLocation; CRGB currentC; CRGB finalC;};
#define pfire(x) {Serial.print(F("Firework {")); Serial.print(x.location); Serial.print(F(", ")); Serial.print(x.maxSpread); Serial.print(F(", ")); Serial.print(x.lifetime); Serial.print(F(", ")); Serial.print(x.hue); Serial.println("}");};
#define pmove(x) {Serial.print(F("MovingVertex {")); Serial.print(x.location); Serial.print(F(", ")); Serial.print(x.finalLocation); Serial.print(F(", ")); prgb(x.currentC); Serial.print(F(", ")); prgb(x.finalC); Serial.println(F("}"));};

struct RGBRotateData {
    byte speed;
    float red;
    float green;
    float blue;
};

struct RandomData {
    byte speed;
};

struct DotData {
    CRGB color;
    CRGB secondaryColor;
    byte speed;
    float led;
    uint16_t intled;
};

struct HSVPulseData {
    byte hueRate;
    byte pulseSpeed;
    bool sinusoidalDimming;
    byte pulseIntensity;
    float h;
    byte v;
    byte other;
};

struct SolidData {
    CRGB color;
    CRGB oldcolor;
};

struct PulseData {
    CRGB color;
    byte pulseSpeed;
    bool sinusoidalDimming;
    byte pulseIntensity;
    byte i;
};

struct PulseRandomData {
    byte pulseSpeed;
    bool sinusoidalDimming;
};

struct FireworksData {
    byte fireworkCount;
    byte oldfc;
    Firework* fireworks;
};

struct WavesData {
    byte speed;
    byte waveCount;
    boolean random;
    byte colorsLength;
    CRGB *colors;
    MovingVertex *pts;
    byte oldsize;
};

struct ShiftData {
    byte speed;
    byte transitionTime;
    byte colorsLength;
    CRGB *colors;
    float i;
    byte oldIndex;
    byte newIndex;
};

struct BriansFunctionData {
    CRGB color;
    byte pulseSpeed;
    byte pulseIntensity;
    byte i;
};

struct BriansFunction2Data {
    CRGB color;
    CRGB color1;
    CRGB color2;
    CRGB color3;
    byte i;
};

struct BriansFunctionStaticData {
    byte i;
};

union Data {
    RGBRotateData rr;
    RandomData r;
    DotData d;
    HSVPulseData hp;
    SolidData s;
    PulseData p;
    PulseRandomData pr;
    FireworksData f;
    WavesData w;
    ShiftData sh;
    BriansFunctionData bf;
    BriansFunction2Data bf2;
    BriansFunctionStaticData bfs;
} d;


//**LEDOptions**
char *LEDOptions[] = {"RGB Rotate", "Random", "Dot", "HSV Pulse", "Solid", "Pulse", "Pulse Random", "Fireworks", "Waves", "Shift", "Brians Function", "Brians Function 2", "Brians Function Static", "Off"};
enum Mode {RGB_ROTATE, RANDOM, DOT, HSV_PULSE, SOLID, PULSE, PULSE_RANDOM, FIREWORKS, WAVES, SHIFT, BRIANS_FUNCTION, BRIANS_FUNCTION_2, BRIANS_FUNCTION_STATIC, OFF};

//**SETTERS CODE**
void fillInArgs(Mode selected, ESP8266WebServer &server) {
    char buff[7];
    char nameBuff[4];
    switch (selected) {
    case RGB_ROTATE:
        d.rr.speed = (byte) server.arg("p0").toInt();
        break;
    case RANDOM:
        d.r.speed = (byte) server.arg("p0").toInt();
        break;
    case DOT:
        server.arg("p0").substring(1).toCharArray(buff, 7);
        d.d.color = CRGB(strtoul(buff, NULL, 16));
        server.arg("p1").substring(1).toCharArray(buff, 7);
        d.d.secondaryColor = CRGB(strtoul(buff, NULL, 16));
        d.d.speed = (byte) server.arg("p2").toInt();
        break;
    case HSV_PULSE:
        d.hp.hueRate = (byte) server.arg("p0").toInt();
        d.hp.pulseSpeed = (byte) server.arg("p1").toInt();
        d.hp.sinusoidalDimming = server.arg("p2").equals("true");
        d.hp.pulseIntensity = (byte) server.arg("p3").toInt();
        break;
    case SOLID:
        server.arg("p0").substring(1).toCharArray(buff, 7);
        d.s.color = CRGB(strtoul(buff, NULL, 16));
        break;
    case PULSE:
        server.arg("p0").substring(1).toCharArray(buff, 7);
        d.p.color = CRGB(strtoul(buff, NULL, 16));
        d.p.pulseSpeed = (byte) server.arg("p1").toInt();
        d.p.sinusoidalDimming = server.arg("p2").equals("true");
        d.p.pulseIntensity = (byte) server.arg("p3").toInt();
        break;
    case PULSE_RANDOM:
        d.pr.pulseSpeed = (byte) server.arg("p0").toInt();
        d.pr.sinusoidalDimming = server.arg("p1").equals("true");
        break;
    case FIREWORKS:
        d.f.fireworkCount = (byte) server.arg("p0").toInt();
        break;
    case WAVES:
        d.w.speed = (byte) server.arg("p0").toInt();
        d.w.waveCount = (byte) server.arg("p1").toInt();
        d.w.random = server.arg("p2").equals("true");
        d.w.colorsLength = (byte) server.arg("p3").toInt();
        delete[] d.w.colors;
        d.w.colors = new CRGB[d.w.colorsLength]();
        for(byte i = 0; i < d.w.colorsLength; i++) { 
            sprintf(nameBuff, "4s%d", i);
            server.arg(nameBuff).substring(1).toCharArray(buff, 7);
            d.w.colors[i] = CRGB(strtoul(buff, NULL, 16));
        }
        break;
    case SHIFT:
        d.sh.speed = (byte) server.arg("p0").toInt();
        d.sh.transitionTime = (byte) server.arg("p1").toInt();
        d.sh.colorsLength = (byte) server.arg("p2").toInt();
        delete[] d.sh.colors;
        d.sh.colors = new CRGB[d.sh.colorsLength]();
        for(byte i = 0; i < d.sh.colorsLength; i++) { 
            sprintf(nameBuff, "3s%d", i);
            server.arg(nameBuff).substring(1).toCharArray(buff, 7);
            d.sh.colors[i] = CRGB(strtoul(buff, NULL, 16));
        }
        break;
    case BRIANS_FUNCTION:
        server.arg("p0").substring(1).toCharArray(buff, 7);
        d.bf.color = CRGB(strtoul(buff, NULL, 16));
        d.bf.pulseSpeed = (byte) server.arg("p1").toInt();
        d.bf.pulseIntensity = (byte) server.arg("p2").toInt();
        break;
    case BRIANS_FUNCTION_2:
        server.arg("p0").substring(1).toCharArray(buff, 7);
        d.bf2.color = CRGB(strtoul(buff, NULL, 16));
        server.arg("p1").substring(1).toCharArray(buff, 7);
        d.bf2.color1 = CRGB(strtoul(buff, NULL, 16));
        server.arg("p2").substring(1).toCharArray(buff, 7);
        d.bf2.color2 = CRGB(strtoul(buff, NULL, 16));
        server.arg("p3").substring(1).toCharArray(buff, 7);
        d.bf2.color3 = CRGB(strtoul(buff, NULL, 16));
        break;
     }
}


//**HTML STRING**
char HTMLTemplate[] = "<!DOCTYPE html>\n\
<html lang='en'>\n\
<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
<head>\n\
<title>LED Controller</title>\n\
<link href='https://fonts.googleapis.com/css2?family=Comfortaa&display=swap' rel='stylesheet'>\n\
<style>\n\
body {\n\
font-family: 'Comfortaa', cursive;\n\
background-color: #cccccc;\n\
Color: #000088;\n\
}\n\
h1 {\n\
font-size: 10vw;\n\
margin-top: 1vw;\n\
margin-bottom: 1vw;\n\
}\n\
h3 {\n\
font-size: 6vw;\n\
margin-bottom: 1vw;\n\
}\n\
select {\n\
width: min(75vw, 600px);\n\
font-size: min(6vw, 30px);\n\
}\n\
</style>\n\
</head>\n\
<body>\n\
<h3><i>Forrest and Brian's</i></h3>\n\
<h1>LED Controller</h1>\n\
<center>\n\
<h2 id=\"loading\">LOADING...</h2>\n\
<label>\n\
<select onchange='change()'>\n\
<option value='0'>RGB Rotate</option><option value='1'>Random</option><option value='2'>Dot</option><option value='3'>HSV Pulse</option><option value='4'>Solid</option><option value='5'>Pulse</option><option value='6'>Pulse Random</option><option value='7'>Fireworks</option><option value='8'>Waves</option><option value='9'>Shift</option><option value='10'>Brians Function</option><option value='11'>Brians Function 2</option><option value='12'>Brians Function Static</option><option value='13'>Off</option>\
</select>\n\
</label>\n\
<div style='margin-bottom: 3vh' id='s'></div>\n\
<i style='font-size: x-large;' id='fps'></i>\n\
</center>\n\
</body>\n\
<script>\n\
let newData = false;\n\
const vlas = {};\n\
setInterval(send, 200);\n\
setInterval(getFPS, 2000);\n\
const inputs = {\n\
                'RGB Rotate': ['<div>Speed: <input type=\"range\" id=\"0\" max=\"255\" value=\"118\" oninput=\"u();\"></div>',1],\n\
                'Random': ['<div>Speed: <input type=\"range\" id=\"0\" max=\"255\" value=\"100\" oninput=\"u();\"></div>',1],\n\
                'Dot': ['<div>Color: <input type=\"color\" id=\"0\" value=\"#ff0000\" oninput=\"u();\"></div><div>Secondary Color: <input type=\"color\" id=\"1\" value=\"#000000\" oninput=\"u();\"></div><div>Speed: <input type=\"range\" id=\"2\" max=\"255\" value=\"118\" oninput=\"u();\"></div>',3],\n\
                'HSV Pulse': ['<div>Hue Rate: <input type=\"range\" id=\"0\" max=\"255\" value=\"118\" oninput=\"u();\"></div><div>Pulse Speed: <input type=\"range\" id=\"1\" max=\"255\" value=\"118\" oninput=\"u();\"></div><div>Sinusoidal Dimming: <input type=\"checkbox\" id=\"2\" checked oninput=\"cIF(2,[\\\'N3\\\']);u();\"></div><div>Pulse Intensity: <input type=\"range\" id=\"3\" max=\"255\" value=\"172\" oninput=\"u();\"></div>',4,()=>{cIF(2,['N3']);}],\n\
                'Solid': ['<div>Color: <input type=\"color\" id=\"0\" value=\"#0f0f0f\" oninput=\"u();\"></div>',1],\n\
                'Pulse': ['<div>Color: <input type=\"color\" id=\"0\" value=\"#0000ff\" oninput=\"u();\"></div><div>Pulse Speed: <input type=\"range\" id=\"1\" max=\"255\" value=\"118\" oninput=\"u();\"></div><div>Sinusoidal Dimming: <input type=\"checkbox\" id=\"2\" checked oninput=\"cIF(2,[\\\'N3\\\']);u();\"></div><div>Pulse Intensity: <input type=\"range\" id=\"3\" max=\"255\" value=\"127\" oninput=\"u();\"></div>',4,()=>{cIF(2,['N3']);}],\n\
                'Pulse Random': ['<div>Pulse Speed: <input type=\"range\" id=\"0\" max=\"255\" value=\"118\" oninput=\"u();\"></div><div>Sinusoidal Dimming: <input type=\"checkbox\" id=\"1\" checked oninput=\"u();\"></div>',2],\n\
                'Fireworks': ['<div>Firework Count: <input type=\"range\" id=\"0\" max=\"30\" value=\"1\" oninput=\"u();\"></div>',1],\n\
                'Waves': ['<div>Speed: <input type=\"range\" id=\"0\" max=\"255\" value=\"118\" oninput=\"u();\"></div><div>Wave Count: <input type=\"range\" id=\"1\" max=\"10\" value=\"1\" oninput=\"u();\"></div><div>Random: <input type=\"checkbox\" id=\"2\" checked oninput=\"cIF(2,[\\\'!3\\\',\\\'!4\\\']);u();\"></div><div>Colors Length: <input type=\"range\" id=\"3\" max=\"10\" value=\"2\" oninput=\"cVLA(3,4);u();\"></div><div id=\"4\"></div>',5,()=>{cIF(2,['!3','!4']);cVLA(3,4);}],\n\
                'Shift': ['<div>Speed: <input type=\"range\" id=\"0\" max=\"255\" value=\"118\" oninput=\"u();\"></div><div>Transition Time: <input type=\"range\" id=\"1\" max=\"100\" value=\"33\" oninput=\"u();\"></div><div>Colors Length: <input type=\"range\" id=\"2\" max=\"10\" value=\"2\" oninput=\"cVLA(2,3);u();\"></div><div id=\"3\"></div>',4,()=>{cVLA(2,3);}],\n\
                'Brians Function': ['<div>Color: <input type=\"color\" id=\"0\" value=\"#0000ff\" oninput=\"u();\"></div><div>Pulse Speed: <input type=\"range\" id=\"1\" max=\"255\" value=\"118\" oninput=\"u();\"></div><div>Pulse Intensity: <input type=\"range\" id=\"2\" max=\"255\" value=\"127\" oninput=\"cIF(2,[\\\'N2\\\']);u();\"></div>',3,()=>{cIF(2,['N2']);}],\n\
                'Brians Function 2': ['<div>Color: <input type=\"color\" id=\"0\" value=\"#0000ff\" oninput=\"u();\"></div><div>Color1: <input type=\"color\" id=\"1\" value=\"#0000ff\" oninput=\"u();\"></div><div>Color2: <input type=\"color\" id=\"2\" value=\"#0000ff\" oninput=\"u();\"></div><div>Color3: <input type=\"color\" id=\"3\" value=\"#0000ff\" oninput=\"u();\"></div>',4],\n\
                'Brians Function Static': ['',0],\n\
                'Off': ['',0],\n\
};\n\
vlas['4'] = 'color';\n\
vlas['3'] = 'color';\n\
\n\
const s = document.getElementsByTagName('select')[0];\n\
const cont = document.getElementById('s');\n\
function change() {\n\
const index = inputs[s.options[s.selectedIndex].text];\n\
cont.innerHTML = index[0];\n\
if(index.length > 2) index[2]();\n\
u();}\n\
function cVLA(itsID, divID) {\n\
const slider = document.getElementById(itsID);\n\
const div = document.getElementById(divID);\n\
const oldLen = div.children.length;\n\
const newLen = parseInt(slider.value);\n\
if (oldLen < newLen)\n\
for (let i = oldLen; i < newLen; i++) {\n\
const t = document.createElement('template');\n\
t.innerHTML = '<input type=\"' + vlas['' + divID] + '\" oninput=\"u()\">';\n\
div.appendChild(t.content);\n\
}\n\
else while (div.children.length > newLen) {\n\
div.removeChild(div.lastChild);\n\
}\n\
}\n\
function cIF(itsID, hidesID) {\n\
const checked = document.getElementById(itsID).checked;\n\
for(let hide of hidesID) {\n\
let ch = checked;\n\
if(hide.substr(0,1)==='!') ch = !ch;\n\
let e = document.getElementById(hide.substr(1));\n\
while(e.tagName !== \"DIV\") e = e.parentElement;\n\
e.style.display = ch ? \"block\" : \"none\";\n\
}\n\
}\n\
function request(url, onload) {\n\
let xhr = new XMLHttpRequest();\n\
xhr.onload = () => onload(xhr.responseText);\n\
xhr.open('GET', url, true);\n\
xhr.send(null);\n\
}\n\
function getFPS() {\n\
request('/gt', (res) => {\n\
document.getElementById('fps').innerHTML = parseFloat(res).toFixed(2) + \" fps\";\n\
});\n\
}\n\
request('/gc', (res) => {\n\
const a = res.split(\"\|\");\n\
s.selectedIndex = parseInt(a[0]);\n\
cont.innerHTML = inputs[s.options[s.selectedIndex].text][0];\n\
for (let i = 0; i < inputs[s.options[s.selectedIndex].text][1]; i++) {\n\
if (document.getElementById('' + i).type === 'checkbox') {\n\
document.getElementById('' + i).checked = (a[i + 1] === \"1\");\n\
} else {\n\
document.getElementById('' + i).value = a[i + 1];\n\
}\n\
}\n\
document.getElementById('loading').remove();\n\
})\n\
function u() { newData = true; }\n\
function send() {\n\
if (newData) {\n\
let xmlHttp = new XMLHttpRequest();\n\
const params = {};\n\
for (let i = 0; i < inputs[s.options[s.selectedIndex].text][1]; i++) {\n\
const e = document.getElementById('' + i);\n\
if(e.type === 'checkbox'){params['p' + i] = e.checked;}\n\
else if (e.tagName === 'DIV') {\n\
for (let j = 0; j < e.children.length; j++) {\n\
params[i + 's' + j] = e.children[j].value;\n\
}\n\
}\n\
else{params['p' + i] = e.value;}\n\
}\n\
xmlHttp.open('GET', '/set?s=' + s.selectedIndex + '&' + new URLSearchParams(params).toString(), true);\n\
xmlHttp.send(null);\n\
newData = false;\n\
}\n\
}\n\
</script>\n\
</html>\n";

//**STRINGIFY PARAMS**
char spBuffer[53];
void stringifyParams(Mode selected) {
    switch (selected) {
    case RGB_ROTATE:
        sprintf(spBuffer, "%i|%i", selected, d.rr.speed);
        break;
    case RANDOM:
        sprintf(spBuffer, "%i|%i", selected, d.r.speed);
        break;
    case DOT:
        sprintf(spBuffer, "%i|#%02X%02X%02X|#%02X%02X%02X|%i", selected, d.d.color.r, d.d.color.g, d.d.color.b, d.d.secondaryColor.r, d.d.secondaryColor.g, d.d.secondaryColor.b, d.d.speed);
        break;
    case HSV_PULSE:
        sprintf(spBuffer, "%i|%i|%i|%i|%i", selected, d.hp.hueRate, d.hp.pulseSpeed, d.hp.sinusoidalDimming, d.hp.pulseIntensity);
        break;
    case SOLID:
        sprintf(spBuffer, "%i|#%02X%02X%02X", selected, d.s.color.r, d.s.color.g, d.s.color.b);
        break;
    case PULSE:
        sprintf(spBuffer, "%i|#%02X%02X%02X|%i|%i|%i", selected, d.p.color.r, d.p.color.g, d.p.color.b, d.p.pulseSpeed, d.p.sinusoidalDimming, d.p.pulseIntensity);
        break;
    case PULSE_RANDOM:
        sprintf(spBuffer, "%i|%i|%i", selected, d.pr.pulseSpeed, d.pr.sinusoidalDimming);
        break;
    case FIREWORKS:
        sprintf(spBuffer, "%i|%i", selected, d.f.fireworkCount);
        break;
    case WAVES:
        sprintf(spBuffer, "%i|%i|%i|%i|%i|", selected, d.w.speed, d.w.waveCount, d.w.random, d.w.colorsLength);
        break;
    case SHIFT:
        sprintf(spBuffer, "%i|%i|%i|%i|", selected, d.sh.speed, d.sh.transitionTime, d.sh.colorsLength);
        break;
    case BRIANS_FUNCTION:
        sprintf(spBuffer, "%i|#%02X%02X%02X|%i|%i", selected, d.bf.color.r, d.bf.color.g, d.bf.color.b, d.bf.pulseSpeed, d.bf.pulseIntensity);
        break;
    case BRIANS_FUNCTION_2:
        sprintf(spBuffer, "%i|#%02X%02X%02X|#%02X%02X%02X|#%02X%02X%02X|#%02X%02X%02X", selected, d.bf2.color.r, d.bf2.color.g, d.bf2.color.b, d.bf2.color1.r, d.bf2.color1.g, d.bf2.color1.b, d.bf2.color2.r, d.bf2.color2.g, d.bf2.color2.b, d.bf2.color3.r, d.bf2.color3.g, d.bf2.color3.b);
        break;
    case BRIANS_FUNCTION_STATIC:
        sprintf(spBuffer, "%i", selected);
        break;
    case OFF:
        sprintf(spBuffer, "%i", selected);
        break;
    }
}

