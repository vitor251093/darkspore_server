(window.webpackJsonp=window.webpackJsonp||[]).push([[49],{1164:function(e,t,a){"use strict";a.r(t);var n=a(228),l=a(229),r=a(231),c=a(230),u=a(233),o=a(232),i=a(3),s=a.n(i),m=a(1125),p=a.n(m),d=a(889),v=a(890),f=a(892),h=a(894),E=a(893),g=a(945),k=a(895),y=a(956),b=function(e){function t(e){var a;Object(n.a)(this,t),a=Object(r.a)(this,Object(c.a)(t).call(this,e));var l=e.match.params.id;a.state={},a.state.userMail=l;var o=Object(u.a)(a);return y.a.dlsApiGet("api.panel.getUserInfo",{mail:l},function(e){o.setState({user:e.data.user})},function(e){}),a}return Object(o.a)(t,e),Object(l.a)(t,[{key:"applyChanges",value:function(e){this.setState({user:p.a.merge(this.state.user,e)})}},{key:"skipTutorial",value:function(){this.applyChanges({account:{creature_rewards:1,level:2,xp:9e3},decks:[{name:"Squad 1",id:1,category:0,slot:1,locked:0,creatures:[]}]}),this.save()}},{key:"save",value:function(){y.a.dlsApiPost("api.panel.setUserInfo",{mail:this.state.userMail},this.state.user,function(e){},function(e){})}},{key:"render",value:function(){var e=this,t=this.state.user;return void 0===t?"Loading...":s.a.createElement("div",{className:"animated fadeIn"},s.a.createElement(d.a,null,s.a.createElement(v.a,{lg:6},s.a.createElement(f.a,null,s.a.createElement(h.a,null,s.a.createElement("strong",null,s.a.createElement("i",{className:"icon-info pr-1"}),"User mail: ",t.email)),s.a.createElement(E.a,null,s.a.createElement(g.a,{responsive:!0,striped:!0,hover:!0},s.a.createElement("tbody",null,s.a.createElement("tr",null,s.a.createElement("td",null,"Name:"),s.a.createElement("td",null,s.a.createElement("strong",null,t.name))),s.a.createElement("tr",null,s.a.createElement("td",null,s.a.createElement("input",{type:"checkbox",checked:t.account.tutorial_completed,onChange:function(t){return e.applyChanges({account:{tutorial_completed:t.target.checked}})}}),"Tutorial completed")),s.a.createElement("tr",null,s.a.createElement("td",null,"Level:"),s.a.createElement("td",null,s.a.createElement("input",{type:"number",value:t.account.level,onChange:function(t){return e.applyChanges({account:{level:parseInt(t.target.value)}})}}))),s.a.createElement("tr",null,s.a.createElement("td",null,"XP:"),s.a.createElement("td",null,s.a.createElement("input",{type:"number",value:t.account.xp,onChange:function(t){return e.applyChanges({account:{xp:parseInt(t.target.value)}})}}))),s.a.createElement("tr",null,s.a.createElement("td",null,"DNA:"),s.a.createElement("td",null,s.a.createElement("input",{type:"number",value:t.account.dna,onChange:function(t){return e.applyChanges({account:{dna:parseInt(t.target.value)}})}}))),s.a.createElement("tr",null,s.a.createElement("td",null,s.a.createElement(k.a,{block:!0,color:"primary",onClick:function(){return e.skipTutorial()}},"Skip Tutorial")),s.a.createElement("td",null,s.a.createElement(k.a,{block:!0,color:"primary",onClick:function(){return e.save()}},"Save"))))))))))}}]),t}(i.Component);t.default=b},956:function(e,t,a){"use strict";var n=a(228),l=a(229),r=a(985),c=a.n(r),u=function(){function e(){Object(n.a)(this,e)}return Object(l.a)(e,null,[{key:"objectToGetParams",value:function(e){var t="";for(var a in e)""!==t&&(t+="&"),t+=a+"="+encodeURIComponent(e[a]);return t}},{key:"dlsApiGet",value:function(t,a,n,l){var r=e.objectToGetParams(a);c.a.get("/dls/api?method="+t+"&"+r).then(function(e){n(e)}).catch(function(e){l(e)})}},{key:"dlsApiPost",value:function(t,a,n,l,r){var u=e.objectToGetParams(a);c.a.post("/dls/api?method="+t+"&"+u,n).then(function(e){l(e)}).catch(function(e){r(e)})}},{key:"gameApiGet",value:function(e,t,a){var n="";for(var l in e)""!==n&&(n+="&"),n+=l+"="+encodeURIComponent(e[l]);c.a.get("/game/api?"+n).then(function(e){t(e)}).catch(function(e){a(e)})}}]),e}();t.a=u}}]);
//# sourceMappingURL=49.381c097e.chunk.js.map