// /index.html
static const char* data_index_html_path PROGMEM = "/index.html";
static const char data_index_html[] PROGMEM = R"rawliteral(
<html>
<head>
<meta charset=UTF-8>
<meta name=viewport content="width=device-width,initial-scale=1">
<title>&#x26A1; MATRIX</title>
<link rel=stylesheet href=https://cdnjs.cloudflare.com/ajax/libs/jquery-modal/0.9.1/jquery.modal.min.css>
<link rel=stylesheet href=https://cdn.jsdelivr.net/npm/@simonwep/pickr/dist/themes/nano.min.css>
<link rel=stylesheet href=style.css>
</head>
<body onselectstart=return!1>
<section class=header>
<h1>&#x1F4A1; Matrix Control</h1>
<div class=settings-link>
&#x2699;
</div>
<div id=settings-modal class=modal>
<div class=container>
<div class=color-picker style=display:none></div>
</div>
<a href=# rel=modal:close>Close</a>
</div>
</section>
<section class=basic-grid>
<div class=card>
<div class=top>&#x1F308;</div>
<div class=bot>Rainbow</div>
</div>
<div class=card>
<div class=top>&#x1F40D;</div>
<div class=bot>Snake</div>
</div>
<div class=card>
<div class=top>&#x2728;</div>
<div class=bot>Sparkle</div>
</div>
<div class=card>
<div class=top>&#x1F506;</div>
<div class=bot>Solid</div>
</div>
<div class=card>
<div class=top>&#x26D4;</div>
<div class=bot>Off</div>
</div>
<div class=card>
<div class=top>&#x1F9F2;</div>
<div class=bot>Chase</div>
</div>
</section>
<script src=https://cdnjs.cloudflare.com/ajax/libs/jquery/3.5.1/jquery.min.js></script>
<script src=https://cdnjs.cloudflare.com/ajax/libs/jquery-modal/0.9.1/jquery.modal.min.js></script>
<script src=https://cdn.jsdelivr.net/npm/@simonwep/pickr/dist/pickr.min.js></script>
<script src=script.js></script>
</body>
</html>)rawliteral";

// /script.js
static const char* data_script_js_path PROGMEM = "/script.js";
static const char data_script_js[] PROGMEM = R"rawliteral($(function(){$(".card").bind("click",function(){let e=$(this).children(".bot").text().toLowerCase();var r=new XMLHttpRequest;r.open("GET","/mode?pattern="+e,!0),r.send()}),$(".settings-link").bind("click",function(){$("#settings-modal").modal({fadeDuration:100})})});const pickr=Pickr.create({el:".color-picker",theme:"nano",swatches:["rgb(244, 67, 54)","rgb(233, 30, 99)","rgb(156, 39, 176)","rgb(103, 58, 183)","rgb(63, 81, 181)","rgb(33, 150, 243)","rgb(3, 169, 244)","rgb(0, 188, 212)","rgb(0, 150, 136)","rgb(76, 175, 80)","rgb(139, 195, 74)","rgb(205, 220, 57)","rgb(255, 235, 59)","rgb(255, 193, 7)"],showAlways:!0,inline:!0,lockOpacity:!0,components:{preview:!0,hue:!0,interaction:{hex:!0,rgba:!0,hsla:!0,input:!0,clear:!1,save:!0}}});)rawliteral";

// /style.css
static const char* data_style_css_path PROGMEM = "/style.css";
static const char data_style_css[] PROGMEM = R"rawliteral(@import url(https://fonts.googleapis.com/css2?family=Noto+Sans:wght@400;700&display=swap);body{background:#131313;color:#fff;font-family:'Noto Sans',sans-serif}h1{font-weight:700}a{text-decoration:none}.header{display:flex;flex-direction:row;justify-content:space-between;align-items:center;width:1hw}.settings-link{font-size:2rem;transition:all 150ms}.settings-link:active{transform:rotate(15deg)}#settings-modal{color:#000}.pickr{display:none}.container{display:flex;justify-content:center}.basic-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(240px,1fr));gap:1.25rem;justify-items:center}.card{display:flex;flex-direction:column;justify-content:center;background:#e6e6e6;background:linear-gradient(180deg,#e6e6e6 0,#b4b4b4 100%);font-size:3rem;color:#131313;box-shadow:rgba(3,8,20,.1) 0 .15rem .5rem,rgba(2,8,20,.1) 0 .075rem .175rem;height:100%;width:100%;border-radius:10px;transition:all 150ms;overflow:hidden;background-size:cover;background-position:center;background-repeat:no-repeat;cursor:pointer}.card:hover{box-shadow:rgba(2,8,20,.1) 0 .35em 1.175em,rgba(2,8,20,.08) 0 .175em .5em;transform:translateY(-1px) scale(1.025)}.bot,.top{padding-left:10px}.top{padding-top:5px;padding-bottom:5px}.bot{font-size:1.75rem;padding-bottom:10px})rawliteral";

