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

    let name = prompt("Enter name: ");
    if (!name.trim()) return false;
    new_item.find('.entity-list-name').text(name);

    let sprite_list_name = $('.sprites-list-ul li').length;
    new_item.find('.entity-list-number').text(`#${sprite_list_name}`);

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