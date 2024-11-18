using Newtonsoft.Json;

public class LeiturasModel
{
    /// <summary>
    /// Lista de propriedades dos gr�ficos
    /// </summary>
    [JsonProperty("charts")]
    public Chart Charts { get; set; }

    /// <summary>
    /// Lista das leituras
    /// </summary>
    [JsonRequired]
    [JsonProperty("readings")]
    public List<Data> Readings { get; set; }


    public class Chart
    {
        /// <summary>
        /// N�mero de leituras exibidas no gr�fico
        /// </summary>
        [JsonProperty("range")]
        public string Range { get; set; }
    }

    public class Data
    {
        //[JsonRequired]
        //[JsonProperty("FirstEmg")]
        //public string FirstEmg { get; set; }

        /// <summary>
        /// Valor da leitura, em mV, da rota FlexEmg que contempla o m�sculo flexor
        /// </summary>
        [JsonRequired]
        [JsonProperty("FlexEmg")]
        public string FlexEmg { get; set; }

        /// <summary>
        /// Valor da leitura, em mV, da rota ExtEmg que contempla o m�sculo extensor
        /// </summary>
        [JsonProperty("ExtEmg")]
        public string ExtEmg { get; set; }

        /// <summary>
        /// Valor da leitura, em mV, da rota PolEmg que contempla o m�sculo do polegar
        /// </summary>
        [JsonProperty("PolEmg")]
        public string PolEmg { get; set; }

        /// <summary>
        /// Hor�rio da leitura
        /// </summary>
        [JsonProperty("timestamp")]
        public string Timestamp { get; set; }
    }
}