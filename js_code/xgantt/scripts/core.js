//var index = 2, max = 3, bannerTimer = null;
//function showBanner(vIndex, stop) {
//    for (var i = 1; i <= max; i++) {        
//        var a = document.getElementById("bannera" + i);
//        var d = document.getElementById("banner" + i);
//        if (!a) return;

//        if (i == vIndex) {
//            d.style.display = "block";
//            a.className = "selected";
//        } else {
//            d.style.display = "none";
//            a.className = "";
//        }
//    }
//    index = vIndex + 1;
//    if (index > max) index = 1;
//    if (stop != true) {
//        startBanner();
//    } else {
//        clearTimeout(bannerTimer);
//    }
//}
//function startBanner() {
//    bannerTimer = setTimeout(function () {
//        showBanner(index);
//    }, 4000);
//}
//startBanner();

function showNews(vIndex) {
    for (var i = 1; i <= 2; i++) {
        var a = document.getElementById("n" + i);
        var d = document.getElementById("news" + i);
        if (i == vIndex) {
            d.style.display = "block";
            a.className = "selected";
        } else {
            d.style.display = "none";
            a.className = "";
        }
    }
}

////////////////////////////////////////////////
var hideTimer = null;
function showMenu() {
    var menu = document.getElementById("popupmenu");
    menu.style.display = "block";
    clearTimeout(hideTimer);

    var a = menu.parentNode.firstChild;
    a.className = "selected";
}
function hideMenu() {

    hideTimer = setTimeout(function () {
    
        var menu = document.getElementById("popupmenu");
        menu.style.display = "none";

        var a = menu.parentNode.firstChild;
        a.className = "";
    }, 500);
}