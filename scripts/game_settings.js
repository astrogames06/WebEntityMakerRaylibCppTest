window.InitGameSettingsUI = function()
{
    $('#game-settings-close').on('click', function() {
        $(".game-settings-part").addClass('hidden');
    });
};