function gs_get_token()
{   // make ajax request for token

    console.log("get gs token");
    function success(data)
    {   // pass token back to client
        // handle login required

        console.log("Token success response");
        if (data['login_required'])
        {
            console.log("Login required");
            Gnomescroll.login_required();
        }
        else
        {
            token = data[Gnomescroll.token_name];
            if (token)
            {
                console.log("Token: " + token);
                Gnomescroll.set_token(token);
            }
            else
            {
                console.log("Token failure");
                Gnomescroll.token_failure();
            }
        }
    }

    function error()
    {
        console.log("Token error response");
        Gnomescroll.token_failure();
    }

    $.ajax({
        method: 'GET',
        url: Gnomescroll.token_url,
        success: success,
        error: error,
        dataType: 'json',
        headers: {
            'X-Requested-With': 'XMLHttpRequest',
        },
        xhrFields: {
            withCredentials: true
        },
    });
}

function gs_submit_form(form, url, success, error)
{
    var data = form.serializeArray();
    $.ajax({
        type: 'POST',
        url: url,
        data: data,
        success: success,
        error: error,
        dataType: 'json',
        headers: {
            'X-Requested-With': 'XMLHttpRequest',
        },
        xhrFields: {
            withCredentials: true
        },
    });
}

function gs_clear_form_errors()
{   // remove html error indicators
    $('div.errors').each(function(i, el) {
        $(el).children().remove();
    });
}

function gs_auth_server_error(jqXHR, textStatus, errorCode)
{
    console.log("Auth server failure.");
    Gnomescroll.set_message("Authentication server failure. Try again soon.");
};

function gs_make_error_element(msg)
{   // create an element for rendering errors
    var el = $('<div class="alert alert-error"></div>');
    el.text(msg);
    return el;
}

function gs_append_form_errors(errors, form_type)
{   // attach errors received from json response to the form
    $.each(errors, function(name, msgs)
    {
        if (name == 'count') return;
        var el = $('div#'+name+'.errors');
        if (name == 'csrf_token')
        {   // we have to set a custom error here, because wtforms
            // does not let one configure the message "CSRF Failed" (which is confusing to most users)
            // in reality this should never happen, as we request the token when the form is submitted
            // If this actually occurs, its certain to be a bug and not an expired csrf token.
            el.append($('<div class="alert alert-error">' + form_type + ' failed due to server error. Resubmit the form, and if this error persists, please notify the developers.</div>'));
        }
        else
        {   // set error as normal
            var len = msgs.length;
            for (var i=0; i<len; i++)
                el.append(gs_make_error_element(msgs[i]));
        }
    });
}

function gs_extract_and_set_token(resp)
{   // extract token from json response, and notify the game client
    var token = resp[Gnomescroll.token_name];
    console.log("Ajax resposne game token: " + token);
    if (token)
        Gnomescroll.set_token(token);
    else // error if token evaluates to false
        Gnomescroll.set_message("Authentication server failure.");
}
