$wnd.rstudio.runAsyncCallback2("function oRd(a){this.a=a}\nfunction sRd(a){this.a=a}\nfunction xRd(a){this.a=a}\nfunction uRd(a,b){this.a=a;this.b=b}\nfunction pKd(a,b){this.a=a;this.b=b}\nfunction qRd(a){this.a=a;Gb.call(this)}\nfunction tdc(a){vdc(a,irc((!hrc&&(hrc=new lrc),O7b(),a.u)))}\nfunction JYc(a){var b;b=new NYc(a.c,a.b,a.a);b.f=a.f;b.d=a.d;b.e=a.e;return b}\nfunction oic(a){var b,c;for(c=0;c<a.Ql();++c){for(b=0;b<a.Pl(c);++b){nic(a,c,b,false)}}}\nfunction HYc(){GYc();var a,b;a=xF(MW,Knh,337,FYc.b.length,0,1);for(b=0;b<a.length;b++)a[b]=JYc(Wbc(FYc,b));return a}\nfunction zRd(a,b,c){ffc();usd.call(this,a,c);this.a=new otc;Ke(this.a,ZRh,SXh);Mlc(this.a,b)}\nfunction MYc(a,b){bLc(b,uXb(a.c)+'');bLc(b,a.b);bLc(b,a.a);bLc(b,a.f+'');if(a.f!=0){bLc(b,kAc(a.e.a));bLc(b,a.d)}FBc(b.b,lth);b.a=true}\nfunction KYc(a){var b,c;if(UAc(a.a,quh))return a.a;c=E1c('\\\\\"method\\\\\":\\\\s*\\\\\"([^\"]+)\\\\\"');b=A1c(c,a.a,0);if(!b)return null;return b.match[1]}\nfunction OYc(a){var b,c,d,e,f,g,h;if(a.length==0||a.length==1&&a[0].length==0)return null;e=vzc(a[0]);d=a[1];c=a[2];h=uzc(a[3]);g=null;f=null;if(h!=0){g=lAc(vzc(a[4]));f=a[5]}b=new NYc(e,d,c);b.f=h;b.e=g;b.d=f;return b}\nfunction wRd(a,b){var c,d,e,f,g,h;h=new WKc(b);c=new fcc;e=new ZKc(h);g=YKc(e)[0];while(e.a<e.b.a.length){f=YKc(e);d=OYc(f);!!d&&(c.b[c.b.length]=d,true)}a.a.e=vzc(g);a.a.b=ecc(c,xF(MW,Knh,337,0,0,1));iRd(a.a,false,true)}\nfunction gRd(a){var b,c,d,e;d=a.a;e=a.d;c=new cjc;(O7b(),c.tb).style[sph]=(Ht(),Kph);b=new Jgc;Cgc(b,'Request ID: '+a.b+'\\n\\n'+'== REQUEST ======\\n'+d+'\\n\\n'+'== RESPONSE ======\\n'+e+lth);b.tb.style[$rh]=Wph;vac(c,b,c.tb);P$b(this,c)}\nfunction iRd(a,b,c){var d,e,f,g,h;if(b){a.b=HYc();a.e=fXb(ai())}qac(a.f);a.j=a.b[0].c;d=rXb(a.e,a.j);h=KF(sXb(d)*a.g);a.o=a.b.length*15;Ke(a.f,h+Xoh,a.o+Xoh);for(g=0,e=a.b.length;g<e;g++){f=a.b[g];hRd(a,g,f)}if(c){wdc(a.i,0);tdc(a.i)}}\nfunction hRd(a,b,c){var d,e,f,g,h,i,j;j=a.o-(b+1)*15;h=KF(sXb(rXb(c.c,a.j))*a.g);f=c.e?c.e.a:a.e;i=tAc(0,KF(sXb(rXb(a.e,f))*a.g)-1);d=c.f==0;g=new Jgc;(O7b(),g.tb).style[sph]=(Ht(),Hph);g.tb.style[$rh]=Vph;Cgc(g,KYc(c)+(d?' (active)':''));d&&(g.tb.style[RXh]=(Xs(),lxh),undefined);switch(c.f){case 2:e='red';break;case 0:e='#f99';break;case 1:e='#88f';break;case 3:e='#E0E0E0';break;case 4:default:e='yellow';}g.tb.style[Cqh]=e;g.tb.style[OCh]=(dr(),zAh);gf(g,new uRd(a,c),(ix(),ix(),hx));amc(a.f,g);nmc(a.f,g,j,(Su(),Ru),15,Ru);imc(a.f,g,h,Ru,i,Ru);g.rb.d.style[sph]=Hph}\nfunction jRd(){var a;this.f=new qmc;wq(ye(this.f).style,'borderRight','2px dashed #888');this.i=new Oud(this.f);xdc(this.i,krh,krh);gf(this.i,new oRd(this),(ix(),ix(),hx));a=new yrc;(O7b(),a.tb).style[Cqh]=axh;a.tb.style[Jqh]='500';vq(a.tb.style,0.9);this.a=new bdc;wq(ye(this.a).style,Cqh,'#FFE');this.d=new Jgc;Igc(this.d,'<p>Click on a request to see details. Click on the background to show these instructions again.<\\/p><h4>Available commands:<\\/h4><ul><li>Esc: Close<\\/li><li>P: Play/pause<\\/li><li>E: Export<\\/li><li>I: Import<\\/li><li>+/-: Zoom in/out<\\/li><\\/ul>');this.a.Bl(this.d);Ygc(a,this.a,200);urc(a,this.i,(yhc(),rhc),0,null);P$b(this,a);this.c=h8b(this);this.n=new qRd(this);iRd(this,true,true)}\nGWb(27,11,Crh);_.jl=function sac(){qac(this)};GWb(235,271,Crh);_.jl=function _fc(){qac(this.T)};GWb(676,27,fsh);_.jl=function Gic(){oic(this)};GWb(124,70,Erh);_.jl=function ejc(){ajc(this)};GWb(2865,2864,QXh);_.jl=function ltc(){throw new VBc('Use TabLayoutPanel.clear() to alter the DeckLayoutPanel')};GWb(1073,1,$Eh);_.Ng=function oKd(){var a,b;b=new jRd;a=Qqc();dmc(a,b,a.D.c);lmc(a,b,10,(Su(),Ru),10,Ru);imc(a,b,10,Ru,10,Ru);hf(b,new pKd(a,b),jz?jz:(jz=new ux))};GWb(1074,1,Tqh,pKd);_.Nh=function qKd(a){fmc(this.a,this.b)};var P1=dzc(1074);GWb(2823,Uqh,Vqh,gRd);var L3=dzc(2823);GWb(2523,Uqh,YTh,jRd);_.Oh=function kRd(a){return hf(this,a,jz?jz:(jz=new ux))};_.kg=function lRd(){Aj((tj(),sj),new sRd(this))};_.Ri=function mRd(a){var b,c,d,e,f,g,h;if(g8b(a.d)==128){g=cq(a.d);if(g==27){mz(this,this,false);qyc(this.c.a)}else if(g==82&&gRc(a.d)==0){iRd(this,true,true)}else if(g==80){if(this.k)Db(this.n);else{iRd(this.n.a,true,false);Fb(this.n,2000)}this.k=!this.k}else if(g==69){h=new cLc;bLc(h,uXb(this.e)+'');FBc(h.b,lth);h.a=true;for(d=this.b,e=0,f=d.length;e<f;++e){c=d[e];MYc(c,h)}b=new zRd(WMh,h.b.a,null);xVc(b)}else if(g==73){b=new zRd('Import','',new xRd(this));xVc(b)}}else if(g8b(a.d)==256){if(cq(a.d)==43){this.g*=2;iRd(this,false,false)}else if(cq(a.d)==45){this.g/=2;iRd(this,false,false)}}};_.lg=function nRd(){Db(this.n)};_.e={l:0,m:0,h:0};_.g=0.02;_.j={l:0,m:0,h:0};_.k=false;_.o=0;var S3=dzc(2523);GWb(2524,1,Prh,oRd);_.zh=function pRd(a){this.a.a.Bl(this.a.d)};var M3=dzc(2524);GWb(2525,101,{},qRd);_.Uf=function rRd(){iRd(this.a,true,false)};var N3=dzc(2525);GWb(2526,1,Joh,sRd);_.Pg=function tRd(){wdc(this.a.i,0);tdc(this.a.i)};var O3=dzc(2526);GWb(2527,1,Prh,uRd);_.zh=function vRd(a){var b;kq(a.a);this.a.a.jl();b=new gRd(this.b);(O7b(),b.tb).style[noh]=krh;b.tb.style[loh]=krh;this.a.a.Bl(b)};var P3=dzc(2527);GWb(2528,1,{},xRd);_.En=function yRd(a){wRd(this,a)};var Q3=dzc(2528);GWb(730,156,Yrh,zRd);_.Er=function ARd(){return Glc(this.a)};_.ro=function BRd(){return this.a};_.Fr=function CRd(a){return true};var R3=dzc(730);GWb(4024,184,gWh);_.jl=function x8f(){kTc(zmd(this.d),sJh)};znh(Ci)(2);\n//# sourceURL=rstudio-2.js\n")
