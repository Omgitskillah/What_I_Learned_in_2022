var sheet_id = "Google Sheets ID";
var ss = SpreadsheetApp.openById(sheet_id);
var sheet = ss.getSheetByName('Form Responses 1');

function find_row(column, content){
  for (var i = 2; i <= sheet.getLastRow(); i++){
    if(content == sheet.getRange(column + i).getValue()){
      return ContentService.createTextOutput(i);
    }
  }
  return ContentService.createTextOutput('0');
}

function update_content(row, new_text )
{
  if((row > 1) && (row <= sheet.getLastRow())){
    ContentService.createTextOutput(sheet.getRange("D" + row).setValue(new_text));
  }
}

function get_content( column, row ){
  if((row > 1) && (row <= sheet.getLastRow())){
    return ContentService.createTextOutput(sheet.getRange(column + row).getValue());
  }
}

function get_row_count(){
  return ContentService.createTextOutput(sheet.getLastRow());
}

function doGet(e){
  var command = e.parameter.command;
  if (command !== undefined){
    if("get_showing" == command)
    {
      // get the currently Showing row
      return find_row("D","Showing");
    }
    else if("get_nex_new" == command)
    {
      // get the next New row
      return find_row("D", "New");
    }
    else if("get_lesson" == command)
    {
      // get the lesson
      var row = e.parameter.row;
      if (row !== undefined){
        return get_content("B", row );
      }
    }
    else if("get_name" == command)
    {
      // get the name
      var row = e.parameter.row;
      if (row !== undefined){
        return get_content("C", row );
      }
    }
    else if("set_to_showing" == command)
    {
      // set to Showing
      var row = e.parameter.row;
      if (row !== undefined){
        return update_content(row, "Showing" );
      }
    }
    else if("set_to_posted" == command)
    {
      // set to Posted
      var row = e.parameter.row;
      if (row !== undefined){
        return update_content(row, "Posted" );
      }
    }
    else if("get_row_count" == command)
    {
      // get the total number of rows
      return get_row_count();
    }
  }
}





