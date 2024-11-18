// Converte epochtime para JavaScript Date object
function epochToJsDate(epochTime) {
    return new Date(epochTime * 1000);
}

// Converte tempo para o formato YYYY/MM/DD HH:MM:SS
function epochToDateTime(epochTime) {
    var epochDate = new Date(epochToJsDate(epochTime));
    var dateTime = epochDate.getFullYear() + "/" +
        ("00" + (epochDate.getMonth() + 1)).slice(-2) + "/" +
        ("00" + epochDate.getDate()).slice(-2) + " " +
        ("00" + epochDate.getHours()).slice(-2) + ":" +
        ("00" + epochDate.getMinutes()).slice(-2) + ":" +
        ("00" + epochDate.getSeconds()).slice(-2);

    return dateTime;
}

// Fun��o para plotar valores nos gr�ficos
function plotValues(chart, timestamp, value) {
    var x = epochToJsDate(timestamp).getTime();
    var y = Number(value);
    if (chart.series[0].data.length > 40) {
        chart.series[0].addPoint([x, y], true, true, true);
    } else {
        chart.series[0].addPoint([x, y], true, false, true);
    }
}

// Elementos DOM
const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector('#authentication-bar');
const deleteButtonElement = document.getElementById('delete-button');
const deleteModalElement = document.getElementById('delete-modal');
const deleteDataFormElement = document.querySelector('#delete-data-form');
const viewDataButtonElement = document.getElementById('view-data-button');
const hideDataButtonElement = document.getElementById('hide-data-button');
const tableContainerElement = document.querySelector('#table-container');
const chartsRangeInputElement = document.getElementById('charts-range');
const loadDataButtonElement = document.getElementById('load-data');
const cardsCheckboxElement = document.querySelector('input[name=cards-checkbox]');
const chartsCheckboxElement = document.querySelector('input[name=charts-checkbox]');

// Elementos DOM para as leituras dos sensores
const cardsReadingsElement = document.querySelector("#cards-div");
const chartsDivElement = document.querySelector('#charts-div');
const flexElement = document.getElementById("emgFlex");
const extElement = document.getElementById("emgExt");
const polElement = document.getElementById("emgPol");
const updateElement = document.getElementById("lastUpdate")

// Controla LOGIN/LOGOUT UI
const setupUI = (user) => {
    if (user) {
        //Ativa elementos UI
        loginElement.style.display = 'none';
        contentElement.style.display = 'block';
        authBarElement.style.display = 'block';
        userDetailsElement.style.display = 'block';
        userDetailsElement.innerHTML = user.email;

        // Pega UID do usu�rio e dados do banco
        var uid = user.uid;
        console.log(uid);

        // Caminho do banco com UID do usu�rio
        var dbPath = 'UsersData/' + uid.toString() + '/readings';
        var chartSath = 'UsersData/' + uid.toString() + '/charts/range';

        // Referencia do banco
        var dbRef = firebase.database().ref(dbPath);
        var chartRef = firebase.database().ref(chartSath);

        // CHARTS
        // N�mero de leituras exibidas nos gr�ficos
        var chartRange = 0;
        // Pega os valores de leitura salvos no banco para plotar
        chartRef.on('value', snapshot => {
            chartRange = Number(snapshot.val());
            console.log(chartRange);
            if (chartRange == 0)
                chartRange = 100;

            // Deleta e recria gr�ficos
            chartF.destroy();
            chartE.destroy();
            chartP.destroy();
            chartF = createFlexChart();
            chartE = createExtChart();
            chartP = createPolChart();

            // Atualiza dos gr�ficos com os novos dados
            dbRef.orderByKey().limitToLast(chartRange).on('child_added', snapshot => {
                var jsonData = snapshot.toJSON(); // Exemplo: {FlexEmg: 25.02, ExtEmg: 50.20, PolEmg: 1008.48, timestamp:1641317355}

                var FlexEmg = jsonData.FlexEmg;
                var ExtEmg = jsonData.ExtEmg;
                var PolEmg = jsonData.PolEmg;
                var timestamp = jsonData.timestamp;

                // Plota valores nos gr�ficos
                plotValues(chartF, timestamp, FlexEmg);
                plotValues(chartE, timestamp, ExtEmg);
                plotValues(chartP, timestamp, PolEmg);
            });
        });

        // Atualiza o banco com o novo valor (input field range)
        chartsRangeInputElement.onchange = () => {
            chartRef.set(chartsRangeInputElement.value);
        };

        //CHECKBOXES
        // Checkbox (Cards para as �ltimas leituras dos sensores)
        cardsCheckboxElement.addEventListener('change', (e) => {
            if (cardsCheckboxElement.checked) {
                cardsReadingsElement.style.display = 'block';
            }
            else {
                cardsReadingsElement.style.display = 'none';
            }
        });

        // Checkbox (Gr�fico para as leituras dos sensores)
        chartsCheckboxElement.addEventListener('change', (e) => {
            if (chartsCheckboxElement.checked) {
                chartsDivElement.style.display = 'block';
            }
            else {
                chartsDivElement.style.display = 'none';
            }
        });

        // CARDS
        // Pega as �ltimas leituras e exibe nos cards
        dbRef.orderByKey().limitToLast(1).on('child_added', snapshot => {
            var jsonData = snapshot.toJSON(); // Exemplo: {FlexEmg: 25.02, ExtEmg: 50.20, PolEmg: 1008.48, timestamp:1641317355}
            var FlexEmg = jsonData.FlexEmg;
            var ExtEmg = jsonData.ExtEmg;
            var PolEmg = jsonData.PolEmg;
            var timestamp = jsonData.timestamp;

            // Atualiza elementos DOM
            flexElement.innerHTML = FlexEmg;
            extElement.innerHTML = ExtEmg;
            polElement.innerHTML = PolEmg;
            updateElement.innerHTML = epochToDateTime(timestamp);
        });

        // DELETE DATA
        // Adiciona evento de espera para deletar dados para abrir modal de confirma��o para deletar dados
        deleteButtonElement.addEventListener('click', e => {
            console.log("Remove data");
            e.preventDefault;
            deleteModalElement.style.display = "block";
        });

        // Adiciona evento de espera para deletar dados
        deleteDataFormElement.addEventListener('submit', (e) => {
            // Deleta dados de leitura (readings)
            dbRef.remove();
        });

        // TABLE
        var lastReadingTimestamp;
        // Cria tabela com as primeiras 100 leituras
        function createTable() {
            // Adiciona as informa��es na tabela
            var firstRun = true;
            dbRef.orderByKey().limitToLast(100).on('child_added', function (snapshot) {
                if (snapshot.exists()) {
                    var jsonData = snapshot.toJSON();
                    console.log(jsonData);
                    var FlexEmg = jsonData.FlexEmg;
                    var ExtEmg = jsonData.ExtEmg;
                    var PolEmg = jsonData.PolEmg;
                    var timestamp = jsonData.timestamp;
                    var content = '';
                    
                    content += '<tr>';
                    content += '<td>' + epochToDateTime(timestamp) + '</td>';
                    content += '<td>' + FlexEmg + '</td>';
                    content += '<td>' + ExtEmg + '</td>';
                    content += '<td>' + PolEmg + '</td>';
                    content += '</tr>';
                    $('#tbody').prepend(content);
                    // Salva o lastReadingTimestamp --> primeiro timestamp dos dados lidos
                    if (firstRun) {
                        lastReadingTimestamp = timestamp;
                        firstRun = false;
                        console.log(lastReadingTimestamp);
                    }
                }
            });
        };

        // append readings to table (after pressing More results... button)
        function appendToTable() {
            var dataList = []; // saves list of readings returned by the snapshot (oldest-->newest)
            var reversedList = []; // the same as previous, but reversed (newest--> oldest)
            console.log("APEND");
            dbRef.orderByKey().limitToLast(100).endAt(lastReadingTimestamp).once('value', function (snapshot) {
                // convert the snapshot to JSON
                if (snapshot.exists()) {
                    snapshot.forEach(element => {
                        var jsonData = element.toJSON();
                        dataList.push(jsonData); // create a list with all data
                    });
                    lastReadingTimestamp = dataList[0].timestamp; //oldest timestamp corresponds to the first on the list (oldest --> newest)
                    reversedList = dataList.reverse(); // reverse the order of the list (newest data --> oldest data)

                    var firstTime = true;
                    // loop through all elements of the list and append to table (newest elements first)
                    reversedList.forEach(element => {
                        if (firstTime) { // ignore first reading (it's already on the table from the previous query)
                            firstTime = false;
                        }
                        else {
                            var FlexEmg = element.FlexEmg;
                            var ExtEmg = element.ExtEmg;
                            var PolEmg = element.PolEmg;
                            var timestamp = element.timestamp;
                            var content = '';

                            content += '<tr>';
                            content += '<td>' + epochToDateTime(timestamp) + '</td>';
                            content += '<td>' + FlexEmg + '</td>';
                            content += '<td>' + ExtEmg + '</td>';
                            content += '<td>' + PolEmg + '</td>';
                            content += '</tr>';
                            $('#tbody').append(content);
                        }
                    });
                }
            });
        }

        viewDataButtonElement.addEventListener('click', (e) => {
            // Ativa elementos DOM
            tableContainerElement.style.display = 'block';
            viewDataButtonElement.style.display = 'none';
            hideDataButtonElement.style.display = 'inline-block';
            loadDataButtonElement.style.display = 'inline-block'
            createTable();
        });

        loadDataButtonElement.addEventListener('click', (e) => {
            appendToTable();
        });

        hideDataButtonElement.addEventListener('click', (e) => {
            tableContainerElement.style.display = 'none';
            viewDataButtonElement.style.display = 'inline-block';
            hideDataButtonElement.style.display = 'none';
        });

        // Se o usu�rio delogar...
    } else {
        // Ativa elementos UI
        loginElement.style.display = 'block';
        authBarElement.style.display = 'none';
        userDetailsElement.style.display = 'none';
        contentElement.style.display = 'none';
    }
}
