function EntityNameExists(name)
{
    return $('.entity-list-name').filter(function() {
        return $(this).text() === name;
    }).length > 0;
}
function AddEntityToList() // Used in the C++
{
    let new_item = $('.sprite-template').clone();
    new_item.removeClass('sprite-template');
    new_item.css('display', 'flex');

    let sprite_list_length = $('.sprites-list-ul li').length;
    let name_num_addition = 0;
    while (EntityNameExists(`Sprite ${name_num_addition}`))
    {
        name_num_addition += 1
    }
    new_item.find('.entity-list-name').text(`Sprite ${sprite_list_length}`)
    new_item.find('.entity-list-number').text(`#${sprite_list_length}`);

    $('.sprites-list-ul').append(new_item);

    return true; // Returns if the function was successful
}
function RemoveEntityFromList(index) // Used in the C++
{
    $('.sprites-list-ul li').eq(index).remove();

    // Re arranges the entity list number
    $('.sprites-list-ul li').each(function(i) {
        $(this).find('.entity-list-number').text(`#${i}`);
    });
}