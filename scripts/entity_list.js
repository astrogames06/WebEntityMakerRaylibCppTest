function AddEntityToList(SpriteName) // Used in the C++
{
    let new_item = $('.sprite-template').clone();
    new_item.removeClass('sprite-template');
    new_item.addClass('sprites-list-li');
    new_item.css('display', 'flex');

    let sprite_list_length = $('.sprites-list-ul li').length;
    new_item.find('.entity-list-name').text(SpriteName)
    new_item.find('.entity-list-number').text(`#${sprite_list_length}`);

    $('.sprites-list-ul').append(new_item);
}
function RemoveEntityFromList(index) // Used in the C++
{
    $('.sprites-list-ul li').eq(index).remove();

    // Re arranges the entity list number
    $('.sprites-list-ul li').each(function(i) {
        $(this).find('.entity-list-number').text(`#${i}`);
    });
}

function UpdateEntityList()
{
    // Loops through the entity list and makes sure each of the names are updated correctly
    $('.sprites-list-ul li').each(function(i) {
        let sprite = Module.get_sprite_by_index(i);
        let name = Module.get_sprite_name(sprite);

        $(this).find('.entity-list-name').text(name);
    });
}