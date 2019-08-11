var DLSClient = {};
DLSClient.rawGetRequest = function(url, callback, errorCallback) {
    var xmlHttp = new XMLHttpRequest(); 
    xmlHttp.onload = function() {
        if (callback !== undefined) callback(xmlHttp.responseText);
    }
    xmlHttp.onerror = function(e) {
        if (errorCallback !== undefined) errorCallback(e, xmlHttp.responseText);
    };
    xmlHttp.open("GET", url, true);
    xmlHttp.send(null);
};
DLSClient.rawPostRequest = function(url, obj, callback, errorCallback) {
    var xmlHttp = new XMLHttpRequest(); 
    xmlHttp.onload = function() {
        if (callback !== undefined) callback(xmlHttp.responseText);
    }
    xmlHttp.onerror = function(e) {
        if (errorCallback !== undefined) errorCallback(e, xmlHttp.responseText);
    };
    xmlHttp.open("POST", url, true);
    xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xmlHttp.send(JSON.stringify(obj));
};
DLSClient.getRequest = function(name, params, callback, errorCallback) {
    if (params !== undefined && typeof params === 'object') {
        var str = [];
        for (var p in params)
            if (params.hasOwnProperty(p)) {
                str.push(encodeURIComponent(p) + "=" + encodeURIComponent(params[p]));
            }
        params = str.join("&");
    }
    DLSClient.rawGetRequest("http://{{host}}/dls/api?method=" + name + (params === undefined ? "" : ("&" + params)), callback, errorCallback);
};
DLSClient.postRequest = function(name, params, callback, errorCallback) {
    DLSClient.rawPostRequest("http://{{host}}/dls/api?method=" + name, params, callback, errorCallback);
};
DLSClient.log = function(object) {
    DLSClient.postRequest("api.game.log", object);
}
