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
        axis_inputs[0].value = current_entity.x;
        axis_inputs[1].value = current_entity.y;

        $('#input-angle-box').val(current_entity.z);

        let SelectedSpriteIndex = Module.selected_sprite_index();
        // alert(SelectedSpriteIndex);
        $('.sprites-list-ul').children().eq(SelectedSpriteIndex).addClass('selected-sprites-list-ul');

        // $('.delete-entity-btn').show(); // shows the option to delete entity
    }
    else
    {
        sprite_editor_inputs_container.addClass('hidden'); // Hides the inputs

        // let SelectedSpriteIndex = Module.selected_sprite_index();
        // $('.sprites-list-ul').children().eq(SelectedSpriteIndex).removeClass('selected-sprites-list-ul');
        // $('.delete-entity-btn').hide(); // hides the option to delete entity
    }

    // Shows the reset camera button
    if (Module.camera_has_been_moved())
    {
        $('#reset-camera-btn').removeClass('hidden');
    }
    else
    {
        $('#reset-camera-btn').addClass('hidden');
    }
    requestAnimationFrame(javascript_game_loop);
}