//#autor: Jakub Kováčik (xkovac58)
function myfunction(adress){
    $('#container').hide(400);
    $.ajax({
    url: "http://127.0.0.1:5000/ajax_" + adress,
    type: 'GET',
    success: function(res) {
        setTimeout(function(){
        $('#container').html(res);
        $('#container').show(400);
        if (adress !== "logout") {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + adress);
            document.title = adress;
        }
        else {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + "login");
            document.title = "login";
        }
        }, 400);
    }
    });
}

function delete_animal(name, adress){
    var msg = confirm('Chcete odebrat zvíře s jménem '+name+"?");
    if  (msg){
        $('#container').hide(400);
        $.ajax({
        url: "http://127.0.0.1:5000/ajax_" + adress,
        type: 'GET',
        success: function(res) {
            setTimeout(function(){
            $('#container').html(res);
            $('#container').show(400);
            if (adress !== "logout") {
                window.history.pushState("", "", "http://127.0.0.1:5000/" + adress);
                document.title = adress;
            }
            else {
                window.history.pushState("", "", "http://127.0.0.1:5000/" + "login");
                document.title = "login";
            }
            }, 400);
        }
        });
    }

}

function delete_caregiver(name, adress){
    var msg = confirm('Chcete odebrat pečovatele s jménem '+name+"?");
    if  (msg){
    $('#container').hide(400);
    $.ajax({
        url: "http://127.0.0.1:5000/ajax_" + adress,
        type: 'GET',
        success: function(res) {
        setTimeout(function(){
            $('#container').html(res);
            $('#container').show(400);
            if (adress !== "logout") {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + adress);
            document.title = adress;
            }
            else {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + "login");
            document.title = "login";
            }
        }, 400);
        }
    });
    }
    
}

function delete_veterinarian(name, adress){
    var msg = confirm('Chcete odebrat veterinaře s jménem '+name+"?");
    if  (msg){
    $('#container').hide(400);
    $.ajax({
        url: "http://127.0.0.1:5000/ajax_" + adress,
        type: 'GET',
        success: function(res) {
        setTimeout(function(){
            $('#container').html(res);
            $('#container').show(400);
            if (adress !== "logout") {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + adress);
            document.title = adress;
            }
            else {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + "login");
            document.title = "login";
            }
        }, 400);
        }
    });
    }
    
}

function delete_request(name, adress){
    var msg = confirm('Chcete smazat požadavek na ošetření zvířete s jménem '+name+"?");
    if  (msg){
    $('#container').hide(400);
    $.ajax({
        url: "http://127.0.0.1:5000/ajax_" + adress,
        type: 'GET',
        success: function(res) {
        setTimeout(function(){
            $('#container').html(res);
            $('#container').show(400);
            if (adress !== "logout") {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + adress);
            document.title = adress;
            }
            else {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + "login");
            document.title = "login";
            }
        }, 400);
        }
    });
    }
    
}

function delete_borrowing(name, adress){
    var msg = confirm('Chcete ověřit výpujčku dobrovolníka s jménem '+name+"?");
    if  (msg){
    $('#container').hide(400);
    $.ajax({
        url: "http://127.0.0.1:5000/ajax_" + adress,
        type: 'GET',
        success: function(res) {
        setTimeout(function(){
            $('#container').html(res);
            $('#container').show(400);
            if (adress !== "logout") {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + adress);
            document.title = adress;
            }
            else {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + "login");
            document.title = "login";
            }
        }, 400);
        }
    });
    }

}

function take_animal(name, adress){
    var msg = confirm('Chcete si přivlastnit zvíře s jménem '+name+"?");
    if  (msg){
        $('#container').hide(400);
        $.ajax({
        url: "http://127.0.0.1:5000/ajax_" + adress,
        type: 'GET',
        success: function(res) {
            setTimeout(function(){
            $('#container').html(res);
            $('#container').show(400);
            if (adress !== "logout") {
                window.history.pushState("", "", "http://127.0.0.1:5000/" + adress);
                document.title = adress;
            }
            else {
                window.history.pushState("", "", "http://127.0.0.1:5000/" + "login");
                document.title = "login";
            }
            }, 400);
        }
        });
    }
}

function adopt_animal(name, adress){
    var msg = confirm('Chcete si adoptovat zvíře s jménem '+name+"?");
    if  (msg){
        $('#container').hide(400);
        $.ajax({
        url: "http://127.0.0.1:5000/ajax_" + adress,
        type: 'GET',
        success: function(res) {
            setTimeout(function(){
            $('#container').html(res);
            $('#container').show(400);
            if (adress !== "logout") {
                window.history.pushState("", "", "http://127.0.0.1:5000/" + adress);
                document.title = adress;
            }
            else {
                window.history.pushState("", "", "http://127.0.0.1:5000/" + "login");
                document.title = "login";
            }
            }, 400);
        }
        });
    }
}

setTimeout(function() {
    $('#success_msg').fadeOut('fast');
}, 4000);


function approve_adoption(name_animal, name_volunteer, adress){
    var msg = confirm('Chcete schválit adopci zvířete' +name_animal+ 'dobrovolníkem '+name_volunteer+"?");
    if  (msg){
    $('#container').hide(400);
    $.ajax({
        url: "http://127.0.0.1:5000/ajax_" + adress,
        type: 'GET',
        success: function(res) {
        setTimeout(function(){
            $('#container').html(res);
            $('#container').show(400);
            if (adress !== "logout") {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + adress);
            document.title = adress;
            }
            else {
            window.history.pushState("", "", "http://127.0.0.1:5000/" + "login");
            document.title = "login";
            }
        }, 400);
        }
    });
    }
}