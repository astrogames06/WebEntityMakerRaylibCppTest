window.InitGameSettingsUI = function()
{
    $('#game-settings-close').on('click', function() {
        $(".game-settings-part").addClass('hidden');
    });
};

function ShowGridCheckboxOn()
{
    return $("#showgrid-checkbox").is(":checked");
}

function ShowInfoCheckboxOn()
{
    return $("#showinfo-checkbox").is(":checked");
}