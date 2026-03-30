$(document).ready(function() {
    $('input[type="number"]').on('keydown', function(e) {
        if (["e", "E", "+", "-", "."].includes(e.key)) {
            e.preventDefault();
        }
    });

    // Only reset when user leaves the field
    $('input[type="number"]').on('blur', function() {
        if ($(this).val() === "") {
            $(this).val(0);
        }
    });
});