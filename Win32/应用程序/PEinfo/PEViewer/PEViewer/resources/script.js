// 为Number添加一个原型方法
Number.prototype.toFormatString = function ( base,length ) {   // 转化为达到length长度的字符串，如果不够，则前补零(base为将要转换的进制数)
    if (!base)
        base = 16;
    var temp = this.toString(base);
    if (!length)
        length = temp.length;
    while (temp.length < length) {
        temp = '0' + temp;
    }
    return temp.toUpperCase();
}

Date.prototype.toFormatString = function ( formatString ){
    var year = this.getFullYear();
    var month = this.getMonth() + 1;
    var day = this.getDate();
    var hour = this.getHours();
    var minute = this.getMinutes();
    var second = this.getSeconds();
    
    var str = '';
    if ( !formatString )
        formatString = 'y-m-d h:M:s';
    for ( var i = 0 ; i < formatString.length ; i ++ ){
        switch(formatString.charAt(i)){
        case 'y':   str += Number(year).toFormatString(10,2);  break;
        case 'm':   str += Number(month).toFormatString(10, 2); break;
        case 'd':   str += Number(day).toFormatString(10, 2); break;
        case 'h':   str += Number(hour).toFormatString(10, 2); break;
        case 'M':   str += Number(minute).toFormatString(10, 2); break;
        case 's':   str += Number(second).toFormatString(10, 2); break;
        case '-':
        case ':':
        case ' ':
            str += formatString.charAt(i);
        }
    }
    return str;
}


// 取得节的开始地址和大小
function SectionInfo(){};
SectionInfo.prototype.sectionInfo = []; 
SectionInfo.prototype.init = function(sectionData){
    var data = sectionData ? sectionData : PESectionData;
    for ( var i = 0 ; i < data.length ; i ++ ){
        var section = data[i]['value'];
        var va,fa,size;
        for ( var j = 0 ; j < section.length ; j ++ ){
            if ( section[j]['name'] == 'VirtualAddress' )
                va = section[j]['value'];
            if ( section[j]['name'] == 'SizeOfRawData' )
                size = section[j]['value'];
            if ( section[j]['name'] == 'PointerToRawData' )
                fa = section[j]['value'];    
        }
        this.sectionInfo.push( { virtualAddress:va,size:size,fileAddress:fa } );
    }
}
SectionInfo.prototype.rvaToFoa = function( rva ){
	for( var i=0 ; i<this.sectionInfo.length ; i++){
	    if ( rva >= this.sectionInfo[i]['virtualAddress'] && 
	            rva <= this.sectionInfo[i]['virtualAddress'] + this.sectionInfo[i]['size'] )			
			return rva-this.sectionInfo[i]['virtualAddress']+this.sectionInfo[i]['fileAddress'];
	}
    return -1;
}

var sectionInfo = new SectionInfo();
sectionInfo.init();

function transRVAToFoa(){
    var value = document.getElementById('RVA').value;
    document.getElementById('RVA').value = value.toUpperCase();
    
    var foa = sectionInfo.rvaToFoa( parseInt( value,16 ) );
    document.getElementById('FOA').value = ( foa == -1 ) ? 'Can not found in Section' : '0x ' + foa.toFormatString(16);
}
function translateTimeStamp( timeStamp ){
    var date = new Date( timeStamp * 1000 );
    return date.toFormatString( 'y-m-d h:M:s' );
}

var headHeight = 30;
var leftWidth = 280;

window.onload = function (){   

    var sectionTranslateDiv = document.createElement('div');
    sectionTranslateDiv.id = 'TranslateDiv';
    sectionTranslateDiv.innerHTML = '<div>'+ PEFilePath + '</div>';

    var navigation = document.createElement('div');
    navigation.id = 'Navigation';
    navigation.style.width = leftWidth + 'px';
    navigation.style.float = 'left';
    navigation.innerHTML += '<div>' +
                                '<label for="RVA">RVA：</label>' +
                                '<input type="text" id="RVA" name="RVA" onkeyup="transRVAToFoa()">' +
                            '</div>';
    navigation.innerHTML += '<div style="margin-bottom:20px;">' +
                                '<label for="FOA">FOA：</label>' +
                                '<input type="text" id="FOA" name="FOA" readonly="readonly">' +
                            '</div>';

    var tableContainer = document.createElement('div');
    tableContainer.id = 'TableDiv';
    tableContainer.style.overflowY = 'scroll';
    tableContainer.style.overflowX = 'hidden';

    document.body.appendChild(sectionTranslateDiv);
    document.body.appendChild(navigation);
    document.body.appendChild(tableContainer);
    
    onResize();
    window.onresize = onResize;
    
    for (var i = 0 ; i < PEBaseData.length ; i++)
    {
		showBaseData( PEBaseData[i],tableContainer );
		tableContainer.appendChild( document.createElement('br') );
		tableContainer.appendChild( document.createElement('br') );
	}
    for (var i = 0 ; i < PESectionData.length ; i++)
    {
		showBaseData( PESectionData[i],tableContainer );
		tableContainer.appendChild( document.createElement('br') );
		tableContainer.appendChild( document.createElement('br') );
	}
	if (PEImportData.length != 0)
	{
	    showImportData(PEImportData, tableContainer);
	    tableContainer.appendChild(document.createElement('br'));
	    tableContainer.appendChild(document.createElement('br'));
	}
	if (PEDelayImportData.length != 0) {
	    showImportData(PEDelayImportData,tableContainer,"延迟导入表");
	    tableContainer.appendChild(document.createElement('br'));
	    tableContainer.appendChild(document.createElement('br'));
	}
	if (PEExportData.length != 0)
	{
	    showExportData(PEExportData, tableContainer);
	    tableContainer.appendChild(document.createElement('br'));
	    tableContainer.appendChild(document.createElement('br'));
	}
	if (PERelocationData.length != 0) {
	    showRelocationData(PERelocationData, tableContainer);
	    tableContainer.appendChild(document.createElement('br'));
	    tableContainer.appendChild(document.createElement('br'));
	}
	
	initNavigation();


    function onResize(){
        sectionTranslateDiv.style.width = window.innerWidth.toString() + 'px';
        sectionTranslateDiv.style.height = headHeight+'px';
        navigation.style.height = Number(window.innerHeight - headHeight).toString() + 'px';

        tableContainer.style.width = '0px';
        tableContainer.style.height = '0px';
        tableContainer.style.display = 'none';

        setTimeout(function () {
            tableContainer.style.width = Number(window.innerWidth - 40 - leftWidth).toString() + 'px';
            tableContainer.style.height = Number(window.innerHeight - headHeight - 40).toString() + 'px';
            tableContainer.style.display = 'block';
        }, 0);
    }
}
function initNavigation() {
    var tableDiv = document.getElementById('TableDiv');
    var navigation = document.getElementById('Navigation');
    var navi = [];
    for (var i = 0 ; i < tableDiv.children.length ; i++)
        if (tableDiv.children[i].tagName.toLowerCase() == 'table') {

            var element = tableDiv.children[i].children[0];
            var str = (element.title) ? element.title : element.innerHTML;
            navi[str] = tableDiv.children[i];

            var divNode = document.createElement('div');
            divNode.appendChild(document.createTextNode(str));
            navigation.appendChild(divNode);
        }
    navigation.onclick = function (event) {
        var str = (event.srcElement.title) ? event.srcElement.title : event.srcElement.innerHTML;
        navi[str] && navi[str].scrollIntoView();
    }
}

function showBaseData(data,container){
	var table = document.createElement('table');
	container.appendChild( table );
	
	table.cellSpacing = 0;
	table.width = '100%';
	
	// 标题
	var caption = document.createElement('caption');
    caption.appendChild(document.createTextNode( data['name'] )); 
    caption.title = data['title'];

	var tbody = document.createElement('tbody');

	table.appendChild(caption);	
	table.appendChild( tbody );
	
	tbody.insertRow(0);		
	
	var row, cell;

	var i,j;
	for ( i = 0 ; i < data['value'].length ; i ++ ){
	    row = tbody.insertRow();

		j = 0 ;
		for ( var prop in data['value'][i] ){
		
			var str = data['value'][i][prop];
			
			// 转换以value开头的字符串
			var addr = prop.indexOf('value');
			if ( addr >= 0 && (typeof str === 'number') ){
			    var detail = prop.slice(5);
			    var length = data['value'][i]['length'];
			    if ( detail.length == 0 )
			        str = '0x ' + parseInt(str, 10).toFormatString(16,length );
	            else
			        str = detail + '：' + '0x ' + parseInt(str, 10).toFormatString(16, length);
			}
			
			// 定位 TimeDateStamp ， 转换时间戳为确定的时间
			if ( prop == 'desc' && data['value'][i]['name'] == 'TimeDateStamp' ){
			    str += '：' + translateTimeStamp( parseInt( data['value'][i]['value'],10 ) );
			}
			
			j++;
			row.insertCell().appendChild( document.createTextNode( str ) );
		}
		row.cells[row.cells.length-1].title = row.cells[ 0 ].innerHTML + '：' + row.cells[ 1 ].innerHTML;
	}
	
	// 插入表头
	var th1 = document.createElement('th');
	var th2 = document.createElement('th');
	var th3 = document.createElement('th');
	var th4 = document.createElement('th');

	th1.appendChild( document.createTextNode('属性名') );
	th2.appendChild( document.createTextNode('属性值') );
	th3.appendChild( document.createTextNode('变量长度') );
	th4.appendChild( document.createTextNode('备注') );	
    th1.width = '15%';
    th2.width = Number((j-3)*15).toString() + '%';
	th2.colSpan = Number(j-3).toString() ;
	th3.width = '10%';
	
	tbody.rows[0].appendChild( th1 );
	tbody.rows[0].appendChild( th2 );
	tbody.rows[0].appendChild( th3 );
	tbody.rows[0].appendChild( th4 );
}		

function showImportData(data,container,tableName){
    var table = document.createElement('table');
    var caption = document.createElement('caption');
	var tbody = document.createElement('tbody');

    table.width = '100%';
	table.cellSpacing = 0;
	caption.appendChild(document.createTextNode( tableName == null ? '导入表' : tableName )); 

    container.appendChild( table );
	table.appendChild(caption);	
	table.appendChild( tbody );
	
	var cell;
	var row;

	row = tbody.insertRow();
	var th1 = document.createElement('th');
	var th2 = document.createElement('th');
	var th3 = document.createElement('th');
	th1.appendChild(document.createTextNode('模块名'));
	th2.appendChild(document.createTextNode('函数编号'));
	th3.appendChild(document.createTextNode('函数名'));
	row.appendChild(th1);
	row.appendChild(th2);
	row.appendChild(th3);


	for (var i = 0 ; i < data.length ; i++) {
	    var length = data[i]['value'].length;

        row = tbody.insertRow();
        cell = row.insertCell();
        cell.rowSpan = length;
	    cell.appendChild(document.createTextNode(data[i]['name']));
	    
	    var moduleName = data[i]['name'];

	    for (var j = 0 ; j < length ; j++) {
            if ( j != 0 )
    	        row = tbody.insertRow();

	        for (var prop in data[i]['value'][j]) {
	            var str = data[i]['value'][j][prop];
	            if (typeof str === 'number') {
	                str = '0x ' + parseInt(str, 10).toFormatString(16);
	            }
	            row.insertCell().appendChild(document.createTextNode(str));
	        }
	        row.cells[row.cells.length - 1].title = moduleName;
	    }
	}
}

function showExportData(data, container) {
    var table = document.createElement('table');
    var caption = document.createElement('caption');
    var tbody = document.createElement('tbody');

    table.width = '100%';
    table.cellSpacing = 0;
    caption.appendChild(document.createTextNode('导出表 : ' + data[0]['name']));

    container.appendChild(table);
    table.appendChild(caption);
    table.appendChild(tbody);

    var row;

    row = tbody.insertRow();
    var th1 = document.createElement('th');
    var th2 = document.createElement('th');
    var th3 = document.createElement('th');
    th1.appendChild(document.createTextNode('函数编号'));
    th2.appendChild(document.createTextNode('函数名'));
    th3.appendChild(document.createTextNode('偏移地址（RVA）'));
    row.appendChild(th1);
    row.appendChild(th2);
    row.appendChild(th3);

    for (var i = 0 ; i < data[0]['value'].length ; i++) {
        row = tbody.insertRow();

        for (var prop in data[0]['value'][i]) {
            var str = data[0]['value'][i][prop];
            if (typeof str === 'number') {
                str = '0x ' + parseInt(str, 10).toFormatString(16);
            }
            row.insertCell().appendChild(document.createTextNode(str));
        }
    }
}

function showRelocationData(data, container)
{
    var table = document.createElement('table');
    var caption = document.createElement('caption');
    var tbody = document.createElement('tbody');

    table.width = '100%';
    table.cellSpacing = 0;
    caption.appendChild(document.createTextNode('重定位表'));

    container.appendChild(table);
    table.appendChild(caption);
    table.appendChild(tbody);

    var row,cell;
    var str;
    var value;
    var oneLineDataNumber = 5;

    for (var i = 0 ; i < data.length ; i++) {
        str = '';
        str += '页基址RVA：0x ' + parseInt(data[i]['BaseAddrRVA'],10).toFormatString(16);
        str += '<span>';
        str += '重定位块大小：0x ' + parseInt(data[i]['SizeOfBlock'], 10).toFormatString(16);
        str += '</span>';
        str += '<span><span><span>'
        str += '表格中数据的规则：  重定位信息   (VA所指向地址的长度)   RVA';
        str += '</span></span></span>'

        cell = tbody.insertRow().insertCell()
        cell.colSpan = oneLineDataNumber;
        cell.innerHTML = str;

        for (var j = 0 ; j < data[i]['DataArray'].length ; j++) {

            str = '0x ' + parseInt(data[i]['DataArray'][j]['value'], 10).toFormatString(16);
            str += '<span>';
            switch(data[i]['DataArray'][j]['type']){
                case 0: str += '(对齐)';break;
                case 3: str += '(32位)'; break;
                case 10: str += '(64位)'; break;
            }
            str += '</span>';
            str += '<span>';
            str += '0x ' + parseInt(data[i]['BaseAddrRVA'] + data[i]['DataArray'][j]['value'], 10).toFormatString(16);
            str += '</span>';

            if (j % oneLineDataNumber == 0)
                row = tbody.insertRow();

            cell = row.insertCell();
            cell.width = 100/oneLineDataNumber + '%';
            cell.innerHTML = str;
        }
    }
}