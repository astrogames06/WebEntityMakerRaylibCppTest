var Module = {
    onRuntimeInitialized: function () {
        console.log("onRuntimeInitialized ran!");
        InitUI();
        StartUIUpdate();

        requestAnimationFrame(javascript_game_loop);
    },
    print: (function() {
        var element = document.getElementById('output');
        if (element) element.value = ''; // clear browser cache
        return function(text) {
            if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
            console.log(text);
            if (element) {
                element.value += text + "\n";
                element.scrollTop = element.scrollHeight; // focus on bottom
            }
        };
    })(),
    canvas: (function() {
        var canvas = document.getElementById('canvas');
        return canvas;
    })()
};