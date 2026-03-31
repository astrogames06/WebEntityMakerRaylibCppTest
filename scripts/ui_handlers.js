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

                Module.set_current_position_angle(x, y, angle);
            })
        });

        // Prevents the numbers being put below/above their min/max
        // $('input[type="number"]').on('input', function() {
        //     const min = parseInt($(this).attr('min'));
        //     const max = parseInt($(this).attr('max'));

        //     let val = parseInt($(this).val());
        //     if (!isNaN(val))
        //     {
        //         if (val < min) $(this).val(min);
        //         if (val > max) $(this).val(max);
        //     }
        // })
        $("#delete-entity-btn-game").on('click', function() {
            Module.delete_current_entity();
        });

        $(document).on("click", ".delete-entity-btn-list", function () {
            let entity_index = $(this).closest('li').index();

            //alert(entity_index);
            Module.delete_indexed_entity(entity_index);
        })

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
            let elements_list = '.input-axis-box, .add_entity_btn, .delete-entity-btn, .input-name-box';
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