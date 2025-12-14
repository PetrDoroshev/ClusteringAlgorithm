window.onload = function() {
	switchMode("edit");
}

let xChooseValue = 0;
let yChooseValue = 1;
let selectedRow = null;
let selectedCol = null;

function switchMode(mode) {
	document.getElementById("mode-plot").style.display = "none";
	document.getElementById("mode-edit").style.display = "none";
	document.getElementById("mode-dend").style.display = "none";

	if (mode === "edit") {
		document.getElementById("mode-edit").style.display = "block";
	} else if (mode === "plot") {
		document.getElementById("mode-plot").style.display = "block";
	} else {
		document.getElementById("mode-dend").style.display = "block";
	}
	loadData();
}

async function loadData() {
	let response = await fetch("/data");
	let data = await response.json();
	updateColumnSelectors(data[0].data_vector.length)
	renderTable(data);
}

function selectRow(row_id) {
	if (selectedRow != null) {
		[...document.querySelectorAll("#data-table tr")].forEach(tr => {
			tr.classList.remove("selected-row", tr.dataset.id == selectedRow);
		});
	}
	if (selectedRow === row_id) {
		selectedRow = null;
	} else {
		selectedRow = row_id;
		[...document.querySelectorAll("#data-table tr")].forEach(tr => {
			tr.classList.toggle("selected-row", tr.dataset.id == selectedRow);
		});
	}
}

function selectCol(col_id) {
	if (selectedCol != null) {
		let nowSelectedColId = `Col${selectedCol}`;
		[...document.querySelectorAll("#data-table td")].forEach(td => {
			if (td.id === nowSelectedColId) {
				td.classList.remove("selected-col");
			}
		});
	}
	if (selectedCol === col_id) {
		selectedCol = null;
	} else {
		selectedCol = col_id;
		let nowSelectedColId = `Col${selectedCol}`;
		console.log("SELECT");
		console.log(nowSelectedColId);
		[...document.querySelectorAll("#data-table td")].forEach(td => {
			console.log(td.id);
			if (td.id === nowSelectedColId) {
				td.classList.toggle("selected-col");
			}
		});
	}
}

function renderTable(rows) {
	let tbl = document.getElementById("data-table");
	tbl.innerHTML = "";

	if (rows.length === 0) return;

	let dims = rows[0].data_vector.length

	let header = "<tr><th>ID</th>";
	header += "<th>Class</th>";
	for (let i = 0; i < dims; i++) header += `<th onClick="selectCol(${i})">Dim ${i+1}</th>`;
	header += "</tr>";
	tbl.innerHTML += header;

	for (let r of rows) {
		let tr = document.createElement("tr");
		tr = `<tr data-id="${r.row_id}"><td onClick="selectRow(${r.row_id})">${r.row_id}</td>`;

		tr += `<td>${r.row_class ?? ""}</td>`;

		for (let i = 0; i < dims; i++) {
			let val = r.data_vector[i];
			tr += `<td id=Col${i}><input type="number" value="${val}" onchange="updateCell(${r.row_id}, ${i}, this.value)" /></td>`;
		}
		tr += `</tr>`;
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
	})
	.then(response => {
		if (!response.ok) {
			alert("ERROR WHILE PARSING CONTENT")
			return
		}
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
	let chosed_mode = document.getElementById("classify-mode");
	let max_clusters = null;
	let distance = null;

	let param_value = document.getElementById("classify-param-value").value;
	if (chosed_mode.value === "max-clusters") {
		max_clusters = param_value;
	} else {
		distance = param_value;
	};

	await fetch("/classify", {
		method: "POST",
		headers: {"Content-Type": "application/json"},
		body: JSON.stringify({
			"max_clusters": max_clusters,
			"distance": distance,
		})
	})

	if (document.getElementById("mode-plot").style.display === 'block') {
		drawPlot();
	}
	if (document.getElementById("mode-edit").style.display === 'block') {
		loadData();
		renderTable();
	}
}

function updateColumnSelectors(dims) {
	if (dims === 0) return;

	let xSel = document.getElementById("x-col");
	let ySel = document.getElementById("y-col");

	xSel.innerHTML = "";
	ySel.innerHTML = "";

	for (let i = 0; i < dims; i++) {
		if (i === xChooseValue) {
			xSel.innerHTML += `<option selected="selected" value="${i}">Dim ${i}</option>`;
		} else {
			xSel.innerHTML += `<option value="${i}">Dim ${i}</option>`;
		};

		if (i === yChooseValue) {
			ySel.innerHTML += `<option selected="selected" value="${i}">Dim ${i}</option>`;
		} else {
			ySel.innerHTML += `<option value="${i}">Dim ${i}</option>`;
		};
	}
}

async function drawPlot() {
	let response = await fetch("data/");
	let data = await response.json();

	let xDim = parseInt(document.getElementById("x-col").value) ?? 0;
	let yDim = parseInt(document.getElementById("y-col").value) ?? 1;

	xChooseValue = xDim;
	yChooseValue = yDim;

	let x = data.map(r => r.data_vector[xDim])
	let y = data.map(r => r.data_vector[yDim])

	let cls = data.map(r => r.row_class ?? -1);

	let trace = {
		x: x,
		y: y,
		mode: "markers",
		marker: {color: cls, size:12},
		text: cls.map(c => "Class: " + c),
	};

	Plotly.newPlot("plot", [trace], {title: "Scatter Plor"});
}

function openImage() {
	lightGallery(document.getElementById('lightgallery'), {
		plugins: [lgZoom],
		speed: 500,
		download: true,
		backgroundColor: "#FFFFFF",
		infiniteZoom: true,
	});
}

async function removeSelectedRow() {
	if (selectedRow != null) {
		await fetch("/remove_row", {
			method: "POST",
			headers: {"Content-Type": "application/json"},
			body: JSON.stringify({
				"id_to_remove": selectedRow,
			})
		})
		loadData();
	}
}

async function removeSelectedCol() {
	if (selectedCol != null) {
		await fetch("/remove_col", {
			method: "POST",
			headers: {"Content-Type": "application/json"},
			body: JSON.stringify({
				"id_to_remove": selectedCol,
			})
		})
		loadData();
	}
}
