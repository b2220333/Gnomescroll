var Widgets = (function()
{
    function create_option_container(name, label_cls)
    {
        var tag_name = make_tag_name(name);
        var ctr = $('<div>').attr('class', 'row form-inline well')
                            .attr('id', tag_name + '-container');
        var label = create_label(name, label_cls);
        ctr.append(label);
        return ctr;
    }

    function create_label(text, cls)
    {
        cls = cls || 'option-label';
        var row = $('<span>').attr('class', 'span3');
        var label = $('<label>').attr('class', cls)
                                .text(text);
        row.append(label);
        return row;
    }

    function make_tag_name(name)
    {
        return name.toLowerCase().replace(' ', '-');
    }

    function Choices(args)
    {
        var name = args.name;
        var choices = args.choices;
        var value = (args.value === undefined) ? 0 : args.value;
        function bind_callbacks()
        {
            var that = this;
            this.dropdown.children('li').click(function() {
                that.button_label.text($(this).text());
                if (typeof Gnomescroll != "undefined")
                    Gnomescroll.change_setting(args.setting, $(this).text());
            });
        }

        function make_button_container()
        {
            return $('<div>').attr('class', 'input-append btn-group');
        }

        function make_dropdown()
        {
            var ul = $('<ul>').attr('id', this.tag_name)
                              .attr('class', 'dropdown-menu');
            for (var i=0; i<this.choices.length; i++)
            {
                var li = $('<li>');
                var a = $('<a>').attr('href', '#').text(this.choices[i]);
                li.append(a);
                ul.append(li);
            }
            return ul;
        }

        function make_button()
        {
            var a = $('<a>').attr('class', 'btn btn-info dropdown-toggle p')
                            .attr('data-toggle', 'dropdown')
                            .attr('href', '#');
            var label = $('<span>').attr('id', this.tag_name + '-label')
                                   .text(this.choices[value]);
            var caret = $('<span>').attr('class', 'caret');
            this.button_label = label;
            a.append(label).append(caret);
            return a;
        }

        function create()
        {
            this.container = create_option_container(this.name, 'button-option-label');
            this.label = this.container.children();
            this.button_container = make_button_container.call(this);
            this.container.append(this.button_container);
            this.button = make_button.call(this);
            this.button_container.append(this.button);
            this.dropdown = make_dropdown.call(this);
            this.button_container.append(this.dropdown);
            bind_callbacks.call(this);
            return this.container;
        }

        var obj = {
            create: create,
            name: name,
            tag_name: make_tag_name(name),
            label: null,
            container: null,

            choices: choices,
            dropdown: null,
            button: null,
            button_label: null,
            button_container: null
        };

        obj.create();
        return obj;
    }

    function Color(color)
    {
        function componentToHex(c) {
            var hex = c.toString(16);
            return (hex.length == 1) ? "0" + hex : hex;
        }

        return {
            color: color,

            r: function() { return this.color[0]; },
            g: function() { return this.color[1]; },
            b: function() { return this.color[2]; },

            toString: function() {
                return 'rgb(' + this.r() + ', ' + this.g() + ', ' + this.b() + ')';
            },

            toHex: function() {
                return "#" + componentToHex(this.r()) +
                             componentToHex(this.g()) +
                             componentToHex(this.b());
            }
        };
    }

    function ColorPicker(args)
    {
        var name = args.name;
        var value = args.value || [200, 20, 20];
        var color = Color(value);

        function make_button()
        {
            var btn = $('<button>').attr('type', 'submit')
                                   .attr('class', 'btn-blank colorpicker')
                                   .attr('data-color', this.color.toString())
                                   .css('background-color', this.color.toHex())
                                   .text(' ');
            return btn;
        }

        function bind_colorpicker()
        {
            this.button.colorpicker().on('changeColor', function(ev) {
                $(this).css('background-color', ev.color.toHex());
                if (typeof Gnomescroll != "undefined")
                {
                    var rgb = ev.color.toRGB();
                    rgb = [rgb.r, rgb.g, rgb.b];
                    Gnomescroll.change_setting(args.setting, rgb);
                }
            });
        }

        function create()
        {
            this.container = create_option_container(this.name);
            this.button = make_button.call(this);
            this.container.append(this.button);
            bind_colorpicker.call(this);
            return this.container;
        }

        var obj = {
            create: create,
            name: name,
            tag_name: make_tag_name(name),
            container: null,

            button: null,
            color: color
        };

        obj.create();
        return obj;
    }

    function StringInput(args)
    {
        var name = args.name;
        var current_value = args.value;
        function make_input()
        {
            var input = $('<input>').attr('type', 'text')
            if (current_value)
                input.val(current_value);
            input.keyup(function(e) {
                if (typeof Gnomescroll != "undefined")
                    Gnomescroll.change_setting(args.setting, $(this).val());
            });
            return input;
        }

        function create()
        {
            this.container = create_option_container(this.name);
            this.input = make_input.call(this);
            this.container.append(this.input);
            return this.container;
        }

        var obj = {
            create: create,
            name: name,
            tag_name: make_tag_name(name),
            container: null,

            input: null
        };

        obj.create();
        return obj;
    }

    function ToggleButton(args)
    {
        var name = args.name;
        var on = (args.on === undefined) ? 'On' : args.on;
        var off = (args.off === undefined) ? 'Off' : args.off;
        var value = (args.value === undefined) ? true : args.value;

        function make_button()
        {
            var outer = $('<div>').attr('class', 'switch')
                                  .attr('data-on', 'info')
                                  .attr('data-on-label', on)
                                  .attr('data-off-label', off);
            var input = $('<input>').attr('type', 'checkbox');
            if (value)
                input.attr('checked', '');
            outer.append(input);
            outer.bootstrapSwitch();
            outer.on('switch-change', function (e, data) {
                if (typeof Gnomescroll != "undefined")
                    Gnomescroll.change_setting(args.setting, data.value);
            });
            return outer;
        }

        function create()
        {
            this.container = create_option_container(this.name);
            this.button = make_button.call(this);
            this.container.append(this.button);
            return this.container;
        }

        var obj = {
            create: create,
            name: name,
            tag_name: make_tag_name(name),
            container: null,

            button: null
        };

        obj.create();
        return obj;
    }

    function Slider(args)
    {
        var name = args.name;
        var slider_opts = args.slider_opts || {};
        slider_opts.value = (args.value === undefined) ? 0 : args.value;

        function load_widget(opts)
        {
            opts = opts || slider_opts;
            slider_opts.handle = slider_opts.handle || 'square';
            this.slider.slider(slider_opts);
            this.slider.on('slideStop', function(ev) {
                if (typeof Gnomescroll != "undefined")
                    Gnomescroll.change_setting(args.setting, ev.value);
            });
        }

        function make_slider()
        {
            return $('<input>').attr('type', 'text')
                               .attr('class', 'slider');
        }

        function create()
        {
            this.container = create_option_container(this.name);
            this.slider = make_slider.call(this);
            this.container.append(this.slider);
            return this.container;
        }

        var obj = {
            create: create,
            name: name,
            tag_name: make_tag_name(name),
            container: null,

            load_widget: load_widget,
            slider: null
        };

        obj.create();
        return obj;
    }

    return {
        choices: Choices,
        slider: Slider,
        string_input: StringInput,
        color_picker: ColorPicker,
        toggle: ToggleButton
    };

}());

function add_string_input_widget(args)
{
    if (!args.name) console.log("ERROR: String input widget missing name");
    var widget = Widgets.string_input(args);
    $('#main').append(widget.container);
}

function add_toggle_button_widget(args)
{
    if (!args.name) console.log("ERROR: Toggle button missing name");
    var widget = Widgets.toggle(args);
    $('#main').append(widget.container);
}

function add_slider_widget(args)
{
    if (!args.name) console.log("ERROR: Slider missing name");
    var slider = Widgets.slider(args);
    $('#main').append(slider.container);
    // we have to load the slider itself after appending to the dom because
    // the plugin is dumb
    slider.load_widget();
}

function add_color_picker_widget(args)
{
    if (!args.name) console.log("ERROR: Color picker missing name");
    $('#main').append(Widgets.color_picker(args).container);
}

function add_choice_widget(args)
{
    if (!args.name) console.log("ERROR: Choice widget missing name");
    $('#main').append(Widgets.choices(args).container);
}

function add_widget_from_json(item)
{
    var setting = item[0];
    var type = item[1];
    var ui_element = item[2];
    var value = item[3];
    var args = { setting: setting, name: setting, value: value };

    if (type == 'TYPE_STRING')
        add_string_input_widget(args);
    else
    if (type == 'TYPE_FLOAT' || type == 'TYPE_INT')
        add_slider_widget(args)
    else
    if (type == 'TYPE_BOOL')
        add_toggle_button_widget(args);
    else
    if (type == 'TYPE_COLOR')
        add_color_picker_widget(args)
}
