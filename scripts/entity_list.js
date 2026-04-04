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