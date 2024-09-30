    import {initializeApp} from "https://www.gstatic.com/firebasejs/9.15.0/firebase-app.js";
    import {getAnalytics} from "https://www.gstatic.com/firebasejs/9.15.0/firebase-analytics.js";
    import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/9.15.0/firebase-database.js";
    const firebaseConfig = {
        apiKey: "YOUR KEY HERE",
    authDomain: "YOUR AUTH DOMAIN",
    databaseURL: "YOUR DATABASE URL",
    projectId: "YOUR PROJECT IDL",
    storageBucket: "YOUR STROAGE BUCKET",
    messagingSenderId: "YOUR MESSAGING SENSOR ID",
    appId: "YOUR APP ID",
    measurementId: "YOUR MEASUREMENT ID"
  };
    const app = initializeApp(firebaseConfig);
    const analytics = getAnalytics(app);
    const db = getDatabase(app);

const refs = {
  temp: ref(db, 'SmartFarm/Temperature'),
  hum: ref(db, 'SmartFarm/Humidity'),
  heat: ref(db, 'SmartFarm/HeatIndex'),
  light: ref(db, 'SmartFarm/Light'),
  soil: ref(db, 'SmartFarm/Soil')
};

for (const key in refs) {
  onValue(refs[key], (snapshot) => {
    const data = snapshot.val();
    const element = document.getElementById(key);
    element.innerHTML = data;
  });
}

function formatData(data, unit) {
  return data + ' ' + unit;
}

for (const key in refs) {
  onValue(refs[key], (snapshot) => {
    const data = snapshot.val();
    const element = document.getElementById(key);
    let unit = '';
    if (key === 'temp' || key === 'heat') {
      unit = '°C';
    } else if (key === 'hum' || key === 'light' || key === 'soil') {
      unit = '%';
    }
    element.innerHTML = formatData(data, unit);
  });
}

