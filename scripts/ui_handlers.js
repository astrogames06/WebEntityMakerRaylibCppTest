function InitUI()
{
    $(document).ready(function() {
        $('#add_entity_btn').click(() => {
            console.log("Entity Created!");
            Module.create_sprite();
        });

        let size_inputs = $('.input-size-box');
        size_inputs.each((index, element) => {
            $(element).on('input', function() {
                let width = size_inputs[0].value;
                let height = size_inputs[1].value;

                Module.set_window_size(width, height);
            })
        })

        let axis_inputs = $('.input-axis-box');
        // Loop through them
        axis_inputs.each((index, element) => {
            $(element).on('input', function() {
                let x = axis_inputs[0].value;
                let y = axis_inputs[1].value;
                let angle = $('#input-angle-box').val();
                let name = $('#input-name-box').val();

                let entity = Module.get_current_entity();
                entity.x = x; entity.y = y; entity.angle = angle;
                entity.name = name;
            })
        });

        $('.sprites-list-ul').on('click', 'li', function(e) {
            if ($(e.target).is('button, img')) return;

            let index = $(this).index() // Gets index
            let sprite_to_be_selected = Module.get_sprite_by_index(index); // Gets index of li

            Module.set_selected_entity(sprite_to_be_selected); // Sets selected sprite
        });

        // Updates the sprite name and list as you type it in
        $('#input-name-box').on('input', function() {
            let current_entity = Module.get_current_entity();
            let entity_i = Module.get_entity_index(current_entity);

            current_entity.name = $(this).val();
            $(".sprites-list-ul li").eq(entity_i).find('.entity-list-name').text(current_entity.name); // Sets the lists new name
        }); 

        $("#delete-entity-btn-game").on('click', function() {
            Module.delete_current_entity();
        });
        $("#duplicate-entity-btn-game").on('click', function() {
            let current_entity = Module.get_current_entity();

            Module.duplicate_sprite(Module.get_current_entity());

            $(this).find('img').attr('src', 'assets/icons/check.svg'); // Changes icon

            // Makes it go back to normal
            setTimeout(() => {
                $(this).find('img').attr('src', 'assets/icons/copy.svg');
            }, 2000);
        });

        $(document).on("click", ".copy-entity-btn-list", function() {
            let entity_index = $(this).closest('li').index();
            let entity_name = Module.get_sprite_by_index(entity_index).name;

            navigator.clipboard.writeText(entity_name);
            $(this).find('img').attr('src', 'assets/icons/check.svg'); // Changes icon

            // Makes it go back to normal
            setTimeout(() => {
                $(this).find('img').attr('src', 'assets/icons/copy.svg');
            }, 2000);
        });
        $(document).on("click", ".delete-entity-btn-list", function() {
            let entity_index = $(this).closest('li').index();

            //alert(entity_index);
            Module.delete_indexed_entity(entity_index);
        })
        $(document).on("click", ".look-entity-btn-list", function() {
            let entity_index = $(this).closest('li').index();
            let entity_target = Module.get_sprite_by_index(entity_index);

            Module.move_camera_to_entity(entity_target);
        });

        $("#reset-camera-btn").on('click', function() {
            Module.reset_camera();
        });

        // This makes is so that if we click outside the canvas
        // On something else other then a "button" or "input" then it blurs
        function OnCanvasBlur() { 
            Module.on_blur();
        }

        $("#canvas").on('focusout', function(e) {
            const target_element = e.relatedTarget;
            console.log(target_element);

            // Prevents certain elements from blurring canvas
            let elements_list = `
                .input-axis-box,
                .add_entity_btn,
                .delete-entity-btn,
                #input-name-box,
                .copy-entity-btn-list,
                #duplicate-entity-btn-game,
                .look-entity-btn-list,
                .sprites-list-li
            `;
            if (target_element && $(target_element).is(elements_list)) return;

            OnCanvasBlur();
        });

        // Opens the terminal when click
        $('.output-wrapper h1').on('click', function() {
            if ($('#output').css('display') === 'none')
            {
                $('#output').css('display', 'flex')
            }
            else
            {
                $('#output').css('display', 'none')
            }
        });
    });
}