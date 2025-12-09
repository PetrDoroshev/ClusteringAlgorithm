window.onload = function() {
	switchMode("edit");
}

function switchMode(mode) {
	if (mode === "edit") {
		document.getElementById("mode-edit").style.display = "block";
		document.getElementById("mode-plot").style.display = "none";
	} else {
		document.getElementById("mode-plot").style.display = "block";
		document.getElementById("mode-edit").style.display = "none";
	}
	loadData();
}

async function loadData() {
	let response = await fetch("/data");
	let data = await response.json();
	updateColumnSelectors(data[0].data_vector.length)
	renderTable(data);
}

function renderTable(rows) {
	let tbl = document.getElementById("data-table");
	tbl.innerHTML = "";

	if (rows.length === 0) return;

	let dims = rows[0].data_vector.length

	let header = "<tr><th>ID</th>";
	for (let i = 0; i < dims; i++) header += `<th>Dim ${i+1}</th>`;
	header += "<th>Class</th></tr>";
	tbl.innerHTML += header;

	for (let r of rows) {
		let tr = `<tr data-id="${r.row_id}"><td>${r.row_id}</td>`;

		for (let i = 0; i < dims; i++) {
			let val = r.data_vector[i];
			tr += `<td><input type="number" value="${val}" onchange="updateCell(${r.row_id}, ${i}, this.value)" /></td>`;
		}
		tr += `<td>${r.row_class ?? ""}</td></tr>`;
		tbl.innerHTML += tr;
	}
}

async function updateCell(id, dim, value) {
	await fetch("/update_cell", {
		method: "POST",
		headers: {"Content-Type": "application/json"},
		body: JSON.stringify({
			"row_id": id,
			"col_id": dim,
			"new_val": value,
		}),
	});
}

async function addRow() {
	await fetch("/add_row", {
		method: "POST",
		headers: {"Content-Type": "application/json"},
	});
	loadData();
}

async function addCol() {
	await fetch("/add_col", {
		method: "POST",
		headers: {"Content-Type": "application/json"},
	});
	loadData();
}

async function classify() {
	await fetch("/classify")

}

function updateColumnSelectors(dims) {
	if (dims === 0) return;

	let xSel = document.getElementById("x-col");
	let ySel = document.getElementById("y-col");

	xSel.innerHTML = "";
	ySel.innerHTML = "";

	for (let i = 0; i < dims; i++) {
		xSel.innerHTML += `<option value="${i}">Dim ${i}</option>`
		ySel.innerHTML += `<option value="${i}">Dim ${i}</option>`
	}
}

async function drawPlot() {
	let response = await fetch("data/");
	let data = await response.json();

	let xDim = parseInt(document.getElementById("x-col").value) ?? 0;
	let yDim = parseInt(document.getElementById("y-col").value) ?? 1;

	let x = data.map(r => r.data_vector[xDim])
	let y = data.map(r => r.data_vector[yDim])

	let cls = data.map(r => r.class ?? -1);

	let trace = {
		x: x,
		y: y,
		mode: "markers",
		marker: {color: cls, size:12},
		text: cls.map(c => "Class: " + c),
	};

	Plotly.newPlot("plot", [trace], {title: "Scatter Plor"});
}
