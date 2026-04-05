window.InitSpriteSelector = function()
{
    $('#sprite-selector-close').on('click', function() {
        $('.sprite-selector-container-part').addClass('sprite-selector-container-hidden');
    });

    $('#sprite-selector-container').on('click', '.sprite-selector-item', function () {
        // Gets the sprite texture path based on the image
        let sprite_texture_name = $(this).find('img').attr('src');

        // Creates the sprite
        Module.create_sprite(sprite_texture_name);
        // Closes the sprite selector
        $('.sprite-selector-container-part').addClass('sprite-selector-container-hidden');
    });

    // This part adds all the sprites to sprite grid
    //  <button class="sprite-selector-item"><img src="assets/sprites/monkey.png">Monkey</button>
    fetch('sprite_imgs.json')
        .then(response => response.json())
        .then(data => {
            data.forEach(name => {
                console.log(name); // "Bear", "Buffalo", etc.
                
                let new_sprite_button = $(`
                    <button class="sprite-selector-item">
                        <img src="assets/sprites/${name.toLowerCase()}.png">
                        ${name}
                    </button>
                `);

                $('#sprite-selector-items-grid').append(new_sprite_button);
            });
        })
        .catch(error => {
            alert('Error fetching default sprites:', error);
        });
};