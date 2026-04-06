let name_already_shown = false;

function javascript_game_loop()
{
    // console.log("Module.is_sprite_selected(): ", Module.is_sprite_selected());
    let size_inputs = $('.input-size-box');
    let window_size = Module.get_window_size();
    size_inputs[0].value = window_size.x;
    size_inputs[1].value = window_size.y;

    let sprite_editor_inputs_container = $('.sprite-editor-inputs-container');
    let axis_inputs = $('.input-axis-box');

    let is_sprite_selected = Module.is_sprite_selected();
    
    
    $('.sprites-list-ul li').removeClass('selected-sprites-list-ul');
    if (is_sprite_selected)
    {
        sprite_editor_inputs_container.removeClass('hidden') // Shows the inputs
        let current_entity = Module.get_current_entity();

        if (current_entity) // Makes sure entity exists
        {
            $('#input-name-box').val(current_entity.name); // Shows name

            axis_inputs[0].value = current_entity.x;
            axis_inputs[1].value = current_entity.y;

            $('#input-angle-box').val(current_entity.angle); // Shows angle

            // Highlights the entity on the list
            let SelectedSpriteIndex = Module.selected_sprite_index();
            $('.sprites-list-ul').children().eq(SelectedSpriteIndex).addClass('selected-sprites-list-ul');
        }
    }
    else
    {
        sprite_editor_inputs_container.addClass('hidden'); // Hides the inputs
    }
    requestAnimationFrame(javascript_game_loop);
}